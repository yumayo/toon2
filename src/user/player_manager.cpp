#include "player_manager.h"
#include "player.h"
#include "toon_packet.h"
#include "user_default.h"
#include "action.hpp"
using namespace cinder;
namespace user
{
CREATE_CPP( player_manager, Json::Value& root, std::shared_ptr<network::udp_connection>& connection )
{
    CREATE( player_manager, root, connection );
}
bool player_manager::init( Json::Value& root, std::shared_ptr<network::udp_connection>& connection )
{
    app::console( ) << root << std::endl;

    set_name( "player_manager" );

    set_schedule_update( );

    _udp = connection;

    _udp.lock( )->run_action( action::repeat_forever::create( action::sequence::create( action::call_func::create( [ this ]
    {
        Json::Value root;
        root["name"] = "server_connection";
        _udp.lock( )->write( std::make_shared<network::network_object>( user_default::get_instans( )->get_root( )["server"]["address"].asString( ),
                                                                        user_default::get_instans( )->get_root( )["server"]["port"].asInt( ) ), 
                             Json::FastWriter( ).write( root ) );
    } ), action::delay::create( 1.0F ) ) ) );

    _udp.lock( )->set_name( "udp_connection" );
    connection->set_parent( shared_from_this( ) );

    // 他のクライアントが接続を切ったら呼ばれます。
    _udp.lock( )->on_received_named_json.insert( std::make_pair( "close_client", [ this ] ( network::network_handle handle, Json::Value root )
    {
        if ( _clients.lock( )->get_children( ).empty( ) ) return;
        std::shared_ptr<node> target;
        network::network_object info( root["data"]["ip_address"].asString( ), root["data"]["port"].asInt( ) );
        for ( auto& c : _clients.lock( )->get_children( ) )
        {
            if ( **std::dynamic_pointer_cast<player>( c )->get_handle( ) == info )
            {
                target = c;
                break;
            }
        }
        if ( target )
        {
            _clients.lock( )->remove_child( target );
        }
        _udp.lock( )->destroy_client( handle );
    } ) );

    // 新しいクライアントが接続してきたら呼ばれます。
    _udp.lock( )->on_received_named_json.insert( std::make_pair( "new_client", [ this ] ( network::network_handle handle, Json::Value root )
    {
        create_client( root["data"] );
    } ) );
    _udp.lock( )->set_schedule_update( true );

    auto clients = node::create( );
    _clients = clients;
    add_child( clients );

    auto& j = root["data"];
    auto player = player::create( j["ip_address"].asString( ),
                                  j["port"].asInt( ),
                                  j["select_skin_name"].asString( ).empty( ) ? "" : "skin/" + j["select_skin_name"].asString( ) + ".png" );
    _player = player;
    player->set_color( j["color"].asString( ) == "purple" ? ColorA { 0.6F, 0.2F, 0.8F } : ColorA { 0.2F, 0.8F, 0.6F } );
    player->set_name( "own" );
    add_child( player );

    for ( auto& j : root["data"]["clients"] )
    {
        create_client( j );
    }

    return true;
}
std::list<std::shared_ptr<node>>& player_manager::get_clients( )
{
    return _clients.lock( )->get_children( );
}
std::weak_ptr<player>& player_manager::get_player( )
{
    return _player;
}
void player_manager::create_client( Json::Value const & data )
{
    auto enemy = player::create( data["ip_address"].asString( ), data["port"].asInt( ),
                                 data["select_skin_name"].asString( ).empty( ) ? "" : "skin/" + data["select_skin_name"].asString( ) + ".png" );

    enemy->set_color( data["color"].asString( ) == "purple" ? ColorA { 0.6F, 0.2F, 0.8F } : ColorA { 0.2F, 0.8F, 0.6F } );
    enemy->set_name( "enemy" );
    _clients.lock( )->add_child( enemy );
}
}
