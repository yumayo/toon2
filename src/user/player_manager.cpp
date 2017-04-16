#include "player_manager.h"
#include "player.h"
#include "toon_packet.h"
#include "user_default.h"
using namespace cinder;
namespace user
{
void player_manager::create_client( Json::Value const & root_client )
{
    auto enemy = player::create( root_client["ip_address"].asString( ), root_client["port"].asInt( ), "" );
    enemy->set_color( { 0.6F, 0.2F, 0.8F } );
    enemy->set_name( "enemy" );
    _clients.lock( )->add_child( enemy );
}
bool player_manager::init( Json::Value& root, std::shared_ptr<network::udp_connection>& connection )
{
    set_name( "player_manager" );

    set_schedule_update( );

    _udp = connection;
    connection->set_parent( shared_from_this( ) );

    _udp.lock( )->on_received_json = [ this ] ( network::network_handle handle, Json::Value root )
    {
        if ( root["name"] == "new_client" )
        {
            create_client( root["data"] );
        }
        else if ( root["name"] == "game_update" )
        {
            for ( auto& child : _clients.lock( )->get_children( ) )
            {
                std::weak_ptr<player> client = std::dynamic_pointer_cast<player>( child );
                client.lock( )->set_position( vec2( root["data"]["position"][0].asFloat( ),
                                                    root["data"]["position"][1].asFloat( ) ) );
                client.lock( )->set_radius( root["data"]["radius"].asFloat( ) );
            }
        }
        else
        {

        }
    };

    auto clients = node::create( );
    _clients = clients;
    add_child( clients );

    auto& skin_name_value = user_default::get_instans( )->get_root( )["select"];

    auto& j = root["data"];
    auto player = player::create( j["ip_address"].asString( ), j["port"].asInt( ), "" );
    _player = player;
    player->set_color( { 0.2F, 0.8F, 0.6F } );
    player->set_name( "own" );
    add_child( player );

    for ( auto& j : root["data"]["clients"] )
    {
        create_client( j );
    }

    return true;
}
void player_manager::update( float delta )
{
    if ( !_player.lock( ) || _player.expired( ) ) return;

    // 大きさで、描画順を変更。
    _clients.lock( )->get_children( ).sort( [ ] ( std::shared_ptr<node>& a, std::shared_ptr<node>& b )
    {
        std::weak_ptr<player> p_a = std::dynamic_pointer_cast<player>( a );
        std::weak_ptr<player> p_b = std::dynamic_pointer_cast<player>( b );
        return p_a.lock( )->get_radius( ) < p_b.lock( )->get_radius( );
    } );

    // プレイヤー同士の当たり判定。
    // 他のエネミー同士は計算しません。
    for ( auto& child : _clients.lock( )->get_children( ) )
    {
        std::weak_ptr<player> client = std::dynamic_pointer_cast<player>( child );

        if ( _player.lock( ) == client.lock( ) ) continue;

        // 自分の大きさは加味しません。
        if ( distance( _player.lock( )->get_position( ), client.lock( )->get_position( ) )
             < _player.lock( )->get_radius( ) * 0 + client.lock( )->get_radius( ) )
        {
            std::weak_ptr<player> small = _player;
            std::weak_ptr<player> large = client;
            if ( large.lock( )->get_radius( ) < small.lock( )->get_radius( ) )
            {
                swap( small, large );
            }

            if ( small.lock( )->get_radius( ) < large.lock( )->get_radius( ) / 2 )
            {
                if ( !small.lock( )->is_captureing( ) )
                {
                    small.lock( )->captured( large );
                }
            }
        }
    }

    Json::Value root;
    root["name"] = "game_update";
    root["data"]["position"][0] = _player.lock( )->get_position( ).x;
    root["data"]["position"][1] = _player.lock( )->get_position( ).y;
    root["data"]["radius"] = _player.lock( )->get_radius( );

    for ( auto& child : _clients.lock( )->get_children( ) )
    {
        std::weak_ptr<player> client = std::dynamic_pointer_cast<player>( child );
        _udp.lock( )->write( client.lock( )->get_handle( ), root );
    }
}
std::list<std::shared_ptr<node>>& player_manager::get_clients( )
{
    return _clients.lock( )->get_children( );
}
std::weak_ptr<player>& player_manager::get_player( )
{
    return _player;
}
}

