﻿#include "cell_manager.h"
#include "player.h"
#include "user_default.h"
#include "action.hpp"
#include "scene_manager.h"
#include "network.hpp"
#include "ground.h"
#include "title.h"
#include "bullet_manager.h"
#include "boost/lexical_cast.hpp"
using namespace cinder;
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
    _udp_connection = std::dynamic_pointer_cast<network::udp_connection>( dont_destroy_node.lock( )->get_child_by_name( "udp_connection" ) );
    _tcp_connection = std::dynamic_pointer_cast<network::tcp_client>( dont_destroy_node.lock( )->get_child_by_name( "tcp_connection" ) );

    // 他のクライアントが接続を切ったら呼ばれます。
    _tcp_connection.lock( )->on_received_named_json.insert( std::make_pair( "close_client", [ this ] ( Json::Value root )
    {
        // 削除ハンドルを作成。
        auto info = std::make_shared<network::network_object>( root["data"]["ip_address"].asString( ), root["data"]["udp_port"].asInt( ) );

        auto target = std::find_if( _enemys.begin( ), _enemys.end( ), [ info ] ( std::weak_ptr<enemy>& n )
        {
            return **n.lock( )->get_handle( ) == *info;
        } );
        if ( target == _enemys.end( ) ) return;

        auto ground_mgr = std::dynamic_pointer_cast<ground>( _ground.lock( ) );
        ground_mgr->close_player( ( *target ).lock( )->get_color( ) );

        auto bullet_manager = std::dynamic_pointer_cast<user::bullet_manager>( _bullet_manager.lock( ) );
        bullet_manager->close_player( ( *target ).lock( )->get_color( ) );

        // 保存リストの方の削除。
        {
            auto remove_itr = std::remove_if( _enemys.begin( ), _enemys.end( ), [ info ] ( std::weak_ptr<enemy>& n )
            {
                return **n.lock( )->get_handle( ) == *info;
            } );
            _enemys.erase( remove_itr, _enemys.end( ) );
        }
        // 本削除。
        {
            auto remove_itr = std::remove_if( _children.begin( ), _children.end( ), [ info ] ( std::shared_ptr<node>& n )
            {
                if ( auto cell = std::dynamic_pointer_cast<user::cell>( n ) )
                {
                    return **cell->get_handle( ) == *info;
                }
                return false;
            } );
            _children.erase( remove_itr, _children.end( ) );
        }

        // 削除したとサーバーに通知。
        _udp_connection.lock( )->destroy_client( info );
    } ) );

    // 新しいクライアントが接続してきたら呼ばれます。
    _tcp_connection.lock( )->on_received_named_json.insert( std::make_pair( "new_client", [ this ] ( Json::Value root )
    {
        // udpの方にクライアントの情報を渡してあげます。
        _udp_connection.lock( )->regist_client( root["data"]["ip_address"].asString( ), root["data"]["udp_port"].asInt( ) );
        create_enemy( root["data"] );
    } ) );

    // クライアントの捕食が発生したら呼ばれます。
    _tcp_connection.lock( )->on_received_named_json.insert( std::make_pair( "player_capture", [ this ] ( Json::Value root )
    {
        _player.lock( )->capture( root["data"]["score"].asFloat( ) );
    } ) );

    // サーバーから死んでくれと言われたらタイトルに戻ります。
    _tcp_connection.lock( )->on_received_named_json.insert( std::make_pair( "kill", [ this ] ( Json::Value root )
    {
        scene_manager::get_instans( )->replace( title::create( ) );
    } ) );

    // 他のオブジェクトからゲームの更新命令がされたら呼ばれます。
    // 毎フレーム呼ぶのでudpで通信します。
    _udp_connection.lock( )->on_received_named_json.insert( std::make_pair( "game_update", [ this ] ( network::network_handle handle, Json::Value root )
    {
        for ( auto& enemy : _enemys )
        {
            if ( **enemy.lock( )->get_handle( ) == **handle )
            {
                enemy.lock( )->set_position( vec2( root["data"]["position"][0].asFloat( ),
                                                   root["data"]["position"][1].asFloat( ) ) );
                enemy.lock( )->set_radius( root["data"]["radius"].asFloat( ) );
                return;
            }
        }
    } ) );

    return true;
}
void cell_manager::update( float delta )
{
    if ( !_player.lock( ) || _player.expired( ) ) return;

    // 大きさで、描画順を変更。
    get_children( ).sort( [ ] ( std::shared_ptr<node>& a, std::shared_ptr<node>& b )
    {
        std::weak_ptr<cell> p_a = std::dynamic_pointer_cast<cell>( a );
        std::weak_ptr<cell> p_b = std::dynamic_pointer_cast<cell>( b );
        if ( p_a.lock( ) && p_b.lock( ) )
        {
            return p_a.lock( )->get_radius( ) < p_b.lock( )->get_radius( );
        }
        return false;
    } );

    // プレイヤーとエネミーの当たり判定。
    for ( auto& enemy : _enemys )
    {
        // 自分の大きさは加味しません。
        if ( distance( _player.lock( )->get_position( ), enemy.lock( )->get_position( ) )
             < _player.lock( )->get_radius( ) * 0 + enemy.lock( )->get_radius( ) )
        {
            std::weak_ptr<cell> small = _player;
            std::weak_ptr<cell> large = enemy;
            if ( large.lock( )->get_radius( ) < small.lock( )->get_radius( ) ) swap( small, large );
            if ( small.lock( )->get_radius( ) < large.lock( )->get_radius( ) / 2 )
            {
                if ( !small.lock( )->is_captureing( ) )
                {
                    small.lock( )->captured( large );
                }
            }
        }
    }

    if ( !_udp_connection.lock( ) ) return;

    // 最後に自分のポジションなどを相手に送ります。
    {
        Json::Value root;
        root["name"] = "ground";
        root["data"]["position"][0] = _player.lock( )->get_position( ).x;
        root["data"]["position"][1] = _player.lock( )->get_position( ).y;
        root["data"]["radius"] = _player.lock( )->get_radius( );

        auto server_addr = user_default::get_instans( )->get_root( )["server"]["address"].asString( );
        auto server_port = user_default::get_instans( )->get_root( )["server"]["udp_port"].asInt( );
        _udp_connection.lock( )->write( std::make_shared<network::network_object>( server_addr, server_port ), root );
    }
    {
        Json::Value root;
        root["name"] = "game_update";
        root["data"]["position"][0] = _player.lock( )->get_position( ).x;
        root["data"]["position"][1] = _player.lock( )->get_position( ).y;
        root["data"]["radius"] = _player.lock( )->get_radius( );

        for ( auto& enemy : _enemys )
        {
            _udp_connection.lock( )->write( enemy.lock( )->get_handle( ), root );
        }
    }
}
std::list<std::weak_ptr<enemy>>& cell_manager::get_enemys( )
{
    return _enemys;
}
std::weak_ptr<player>& cell_manager::get_player( )
{
    return _player;
}
void cell_manager::set_ground( std::weak_ptr<node> ground )
{
    _ground = ground;
}
void cell_manager::set_bullet_manager( std::weak_ptr<node> bullet_manager )
{
    _bullet_manager = bullet_manager;
}
void cell_manager::remove_all_crown( )
{
    _player.lock( )->remove_crown( );
    for ( auto& enemy : _enemys )
    {
        enemy.lock( )->remove_crown( );
    }
}
void cell_manager::set_all_crown( std::vector<int> const& ids )
{
    for ( int i = 0; i < ids.size( ); ++i )
    {
        if ( auto cell = get_child_by_tag( ids[i] ) )
        {
            auto p = std::dynamic_pointer_cast<user::cell>( cell );
            if ( p->is_crowner( ) ) continue;

            auto s = renderer::sprite_cubic::create( "crown" + boost::lexical_cast<std::string>( i + 1 ) + ".png" );
            s->set_scale( vec2( 2.0F ) );
            p->set_crown( s );
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

    auto pla = player::create( data["ip_address"].asString( ), data["udp_port"].asInt( ),
                               data["select_skin_name"].asString( ).empty( ) ? "" : "skin/" + data["select_skin_name"].asString( ) + ".png" );

    pla->set_color( ColorA( data["color"][0].asFloat( ), data["color"][1].asFloat( ), data["color"][2].asFloat( ) ) );
    pla->set_position( vec2( data["position"][0].asInt( ), data["position"][1].asInt( ) ) );
    pla->set_tag( data["id"].asInt( ) );

    _player = pla;
    add_child( pla );
}
}