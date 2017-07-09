#include "cell_manager.h"
#include "player.h"
#include <treelike/user_default.h>
#include <treelike/action.hpp>
#include <treelike/scene_manager.h>
#include <treelike/network.hpp>
#include "ground.h"
#include "title.h"
#include "bullet_manager.h"
#include "boost/lexical_cast.hpp"
using namespace cinder;
using namespace treelike;
namespace user
{
CREATE_CPP( cell_manager, Json::Value& root )
{
    CREATE( cell_manager, root );
}
bool cell_manager::init( Json::Value& root )
{
    app::console( ) << root << std::endl;

    set_name( "cell_manager" );

    set_schedule_update( );

    create_player( root["data"] );

    for ( auto& j : root["data"]["clients"] )
    {
        create_enemy( j );
    }

    auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );
    _udp_connection = dont_destroy_node->get_child_by_name( "udp_connection" ).dynamicptr<network::udp_connection>( );
    _tcp_connection = dont_destroy_node->get_child_by_name( "tcp_connection" ).dynamicptr<network::tcp_client>( );

    // 他のクライアントが接続を切ったら呼ばれます。
    _tcp_connection->on( "close_client", [ this ] ( Json::Value root )
    {
        app::console( ) << "close_client" << std::endl;
        app::console( ) << root;

        // 削除ハンドルを作成。
        auto info = network::network_handle( root["data"]["ip_address"].asString( ), root["data"]["udp_port"].asInt( ) );

        auto target = std::find_if( _enemys.begin( ), _enemys.end( ), [ info ] ( softptr<enemy>& n )
        {
            return n->get_handle( ) == info;
        } );
        if ( target == _enemys.end( ) ) return;

        auto bullet_manager = _bullet_manager.dynamicptr<user::bullet_manager>( );
        bullet_manager->close_player( ( *target )->get_color( ) );

        auto ground_mgr = _ground.dynamicptr<ground>( );
        ground_mgr->close_player( ( *target )->get_color( ) );

        // 保存リストの方の削除。
        {
            auto remove_itr = std::remove_if( _enemys.begin( ), _enemys.end( ), [ info ] ( softptr<enemy>& n )
            {
                return n->get_handle( ) == info;
            } );
            _enemys.erase( remove_itr, _enemys.end( ) );
        }
        // 本削除。
        {
            auto remove_itr = std::remove_if( _children.begin( ), _children.end( ), [ info ] ( std::shared_ptr<node>& n )
            {
                if ( auto cell = std::dynamic_pointer_cast<user::cell>( n ) )
                {
                    return cell->get_handle( ) == info;
                }
                return false;
            } );
            _children.erase( remove_itr, _children.end( ) );
        }
    } );

    // 新しいクライアントが接続してきたら呼ばれます。
    _tcp_connection->on( "new_client", [ this ] ( Json::Value root )
    {
        create_enemy( root["data"] );
    } );

    // クライアントの捕食が発生したら呼ばれます。
    _tcp_connection->on( "player_capture", [ this ] ( Json::Value root )
    {
        _player->capture( root["data"]["score"].asFloat( ) );
    } );

    // サーバーから死んでくれと言われたらタイトルに戻ります。
    _tcp_connection->on( "kill", [ this ] ( Json::Value root )
    {
        scene_manager::get_instans( )->replace( title::create( ) );
    } );

    // 他のオブジェクトからゲームの更新命令がされたら呼ばれます。
    // 毎フレーム呼ぶのでudpで通信します。
    _udp_connection->on( "game_update", [ this ] ( network::network_handle handle, Json::Value root )
    {
        for ( auto& enemy : _enemys )
        {
            if ( enemy->get_handle( ) == handle )
            {
                enemy->set_position( vec2( root["data"]["position"][0].asFloat( ),
                                                   root["data"]["position"][1].asFloat( ) ) );
                enemy->set_radius( root["data"]["radius"].asFloat( ) );
                return;
            }
        }
    } );

    return true;
}
void cell_manager::update( float delta )
{
    if ( !_player || _player.expired( ) ) return;

    // 大きさで、描画順を変更。
    sort_children( [ ] ( hardptr<node>& a, hardptr<node>& b )
    {
        softptr<cell> p_a = std::dynamic_pointer_cast<cell>( a );
        softptr<cell> p_b = std::dynamic_pointer_cast<cell>( b );
        if ( p_a && p_b )
        {
            return p_a->get_radius( ) < p_b->get_radius( );
        }
        return false;
    } );

    // プレイヤーとエネミーの当たり判定。
    for ( auto& enemy : _enemys )
    {
        // 自分の大きさは加味しません。
        if ( distance( _player->get_position( ), enemy->get_position( ) )
             < _player->get_radius( ) * 0 + enemy->get_radius( ) )
        {
            softptr<cell> small = _player;
            softptr<cell> large = enemy;
            if ( large->get_radius( ) < small->get_radius( ) ) swap( small, large );
            if ( small->get_radius( ) < large->get_radius( ) / 2 )
            {
                if ( !small->is_captureing( ) )
                {
                    small->captured( large );
                }
            }
        }
    }

    if ( !_udp_connection ) return;

    // 最後に自分のポジションなどを相手に送ります。
    {
        Json::Value root;
        root["name"] = "ground";
        root["data"]["position"][0] = _player->get_position( ).x;
        root["data"]["position"][1] = _player->get_position( ).y;
        root["data"]["radius"] = _player->get_radius( );

        auto server_addr = user_default::get_instans( )->get_root( )["server"]["address"].asString( );
        auto server_port = user_default::get_instans( )->get_root( )["server"]["udp_port"].asInt( );
        _udp_connection->write( network::network_handle( server_addr, server_port ), root );
    }
    {
        Json::Value root;
        root["name"] = "game_update";
        root["data"]["position"][0] = _player->get_position( ).x;
        root["data"]["position"][1] = _player->get_position( ).y;
        root["data"]["radius"] = _player->get_radius( );

        for ( auto& enemy : _enemys )
        {
            _udp_connection->write( enemy->get_handle( ), root );
        }
    }
}
std::list<softptr<enemy>>& cell_manager::get_enemys( )
{
    return _enemys;
}
softptr<player>& cell_manager::get_player( )
{
    return _player;
}
void cell_manager::set_ground( softptr<node> ground )
{
    _ground = ground;
}
void cell_manager::set_bullet_manager( softptr<node> bullet_manager )
{
    _bullet_manager = bullet_manager;
}
void cell_manager::remove_all_crown( )
{
    _player->remove_crown( );
    for ( auto& enemy : _enemys )
    {
        enemy->remove_crown( );
    }
}
void cell_manager::set_all_crown( std::vector<int> const& ids )
{
    for ( int i = 0; i < ids.size( ); ++i )
    {
        if ( auto cell = get_child_by_tag( ids[i] ) )
        {
            auto p = cell.dynamicptr<user::cell>( );
            if ( p->is_crowner( ) ) continue;
            p->set_crown( renderer::sprite_cubic::create( "crown" + boost::lexical_cast<std::string>( i + 1 ) + ".png" ) );
        }
    }
}
void cell_manager::create_enemy( Json::Value const & data )
{
    app::console( ) << "create_enemy" << std::endl;
    app::console( ) << data << std::endl;

    auto enemy = enemy::create( data["ip_address"].asString( ), data["udp_port"].asInt( ),
                                data["select_skin_name"].asString( ).empty( ) ? "" : "skin/" + data["select_skin_name"].asString( ) + ".png" );

    enemy->set_color( ColorA( data["color"][0].asFloat( ), data["color"][1].asFloat( ), data["color"][2].asFloat( ) ) );
    enemy->set_position( vec2( data["position"][0].asInt( ), data["position"][1].asInt( ) ) );
    enemy->set_tag( data["id"].asInt( ) );

    _enemys.emplace_back( enemy );
    add_child( enemy );
}
void cell_manager::create_player( Json::Value const & data )
{
    app::console( ) << "create_player" << std::endl;
    app::console( ) << data << std::endl;

    auto pla = player::create( shared_from_this( ), data["ip_address"].asString( ), data["udp_port"].asInt( ),
                               data["select_skin_name"].asString( ).empty( ) ? "" : "skin/" + data["select_skin_name"].asString( ) + ".png" );

    pla->set_color( ColorA( data["color"][0].asFloat( ), data["color"][1].asFloat( ), data["color"][2].asFloat( ) ) );
    pla->set_position( vec2( data["position"][0].asInt( ), data["position"][1].asInt( ) ) );
    pla->set_tag( data["id"].asInt( ) );

    _player = pla;
    add_child( pla );
}
}
