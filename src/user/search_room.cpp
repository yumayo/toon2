#include "search_room.h"
#include "utility/string_utility.h"
#include "scene_manager.h"
#include <boost/lexical_cast.hpp>
namespace user
{
CREATE_CPP( search_room )
{
    CREATE( search_room );
}
bool search_room::init( )
{
    set_name( "search_room" );

    auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );

    auto tcp_server_address = user_default::get_instans( )->get_root( )["server"]["address"].asString( );
    auto tcp_server_port = user_default::get_instans( )->get_root( )["server"]["tcp_port"].asInt( );
    auto tcp_connection = network::tcp_client::create( tcp_server_address, boost::lexical_cast<std::string>( tcp_server_port ) );
    tcp_connection->set_name( "tcp_connection" );
    _tcp_connection = tcp_connection;
    dont_destroy_node.lock( )->add_child( tcp_connection );
    tcp_connection->on_connection = [ this ]
    {
        Json::Value root;
        root["name"] = "check_handle";
        root["data"]["udp_port"] = _udp_connection.lock( )->get_port( );
        _tcp_connection.lock( )->write( Json::FastWriter( ).write( root ) );
        _tcp_connection.lock( )->on_received_json = [ this ] ( Json::Value root )
        {
            if ( root["name"].asString( ) == "id_received" )
            {
                user_default::get_instans( )->get_root( )["id"] = root["data"]["id"].asInt( );

                Json::Value root;
                root["name"] = "find_room";
                root["data"]["id"] = user_default::get_instans( )->get_root( )["id"].asInt( );
                root["data"]["select_skin_name"] = user_default::get_instans( )->get_root( )["select_skin_name"].asString( );
                _tcp_connection.lock( )->write( Json::FastWriter( ).write( root ) );
                _tcp_connection.lock( )->on_received_json = [ this ] ( Json::Value root )
                {
                    if ( root["name"].asString( ) == "founded" )
                    {
                        _tcp_connection.lock( )->on_connect_failed = nullptr;
                        _tcp_connection.lock( )->on_received_json = nullptr;
                        _tcp_connection.lock( )->on_send_failed = nullptr;
                        _tcp_connection.lock( )->on_disconnected = nullptr;
                        _tcp_connection.lock( )->on_connection = nullptr;
                        _tcp_connection.lock( )->set_schedule_update( false );

                        _udp_connection.lock( )->on_received_json = nullptr;
                        _udp_connection.lock( )->set_schedule_update( false );

                        if ( on_founded ) on_founded( root );
                    }
                };
            }
        };
    };

    auto udp_connection = network::udp_connection::create( );
    udp_connection->set_name( "udp_connection" );
    _udp_connection = udp_connection;
    dont_destroy_node.lock( )->add_child( udp_connection );

    tcp_connection->on_connect_failed = [ this ]
    {
        scene_manager::get_instans( )->top( )->set_block_schedule_event( false );
        _tcp_connection.lock( )->remove_from_parent( );
        _udp_connection.lock( )->remove_from_parent( );
        if ( on_not_found ) on_not_found( );
        remove_from_parent( );
    };
    tcp_connection->on_disconnected = [ this ]
    {
        scene_manager::get_instans( )->top( )->set_block_schedule_event( false );
        _tcp_connection.lock( )->remove_from_parent( );
        _udp_connection.lock( )->remove_from_parent( );
        if ( on_not_found ) on_not_found( );
        remove_from_parent( );
    };
    tcp_connection->on_send_failed = [ this ]
    {
        scene_manager::get_instans( )->top( )->set_block_schedule_event( false );
        _tcp_connection.lock( )->remove_from_parent( );
        _udp_connection.lock( )->remove_from_parent( );
        if ( on_not_found ) on_not_found( );
        remove_from_parent( );
    };

    return true;
}
}