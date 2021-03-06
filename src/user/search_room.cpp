﻿#include "search_room.h"
#include <boost/lexical_cast.hpp>
#include "synchronization_objects.h"
using namespace cinder;
using namespace treelike;
namespace user
{
CREATE_CPP( search_room )
{
    CREATE( search_room );
}
bool search_room::init( )
{
    set_name( "search_room" );
    set_schedule_update( );

    auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );

    auto tcp_server_address = user_default::get_instans( )->get_root( )["server"]["address"].asString( );
    auto tcp_server_port = user_default::get_instans( )->get_root( )["server"]["tcp_port"].asInt( );
    auto tcp_connection = network::tcp_client::create( tcp_server_address, tcp_server_port );
    tcp_connection->set_name( "tcp_connection" );
    _tcp_connection = tcp_connection;
    dont_destroy_node->add_child( tcp_connection );
    tcp_connection->on_connection = [ this ]
    {
        Json::Value root;
        root["name"] = "check_handle";
        root["data"]["udp_port"] = _udp_connection->get_port( );
        _tcp_connection->write( Json::FastWriter( ).write( root ) );
        _tcp_connection->on_received_json = [ this ] ( Json::Value root )
        {
            if ( root["name"].asString( ) == "id_received" )
            {
                Json::Value r;
                r["name"] = "find_room";
                r["data"]["id"] = root["data"]["id"].asInt( );

                // 選択しているスキンがきちんとガチャで手に入ったものなのかを判別します。
                // Jsonのskin項目でtrueになっているか判定しているだけですけど。
                auto select_skin_name = user_default::get_instans( )->get_root( )["select_skin_name"].asString( );
                if ( user_default::get_instans( )->get_root( )["skin"].isMember( select_skin_name ) &&
                     user_default::get_instans( )->get_root( )["skin"][select_skin_name].asBool( ) )
                    r["data"]["select_skin_name"] = select_skin_name;
                else
                    r["data"]["select_skin_name"] = "";

                _tcp_connection->write( Json::FastWriter( ).write( r ) );
                _tcp_connection->on_received_json = [ this ] ( Json::Value root )
                {
                    if ( root["name"].asString( ) == "founded" )
                    {
                        _is_founded = true;
                        _root_buffer = root;
                    }
                    else if( root["name"].asString( ) == "bullet_data" )
                    {
                        _bullet_buffer = root;
                    }
                };
            }
        };
        _tcp_connection->on_readed = [ this ] ( char const* data, size_t byte )
        {
            if ( !_is_founded ) return;

            int index = 0;
            char* wriable_data = const_cast<char*>( data );
            header* h = new( wriable_data + index ) header;
            std::string name( h->name );
            index += sizeof( header );

            if ( name == "feed_data" )
            {
                int const number_of_feed = _root_buffer["data"]["number_of_feed"].asInt( );
                _feed_buffer = std::vector<feed_data>( number_of_feed );
                for ( int i = 0; i < number_of_feed; ++i )
                {
                    feed_data* data = reinterpret_cast<feed_data*>( wriable_data + index );
                    memcpy( &_feed_buffer[i], data, sizeof( feed_data ) );
                    index += sizeof( feed_data );
                }
            }
            else if ( name == "ground_data" )
            {
                int const ground_size = _root_buffer["data"]["ground_size"].asInt( );
                _ground_buffer = std::vector<std::vector<ground_data>>( ground_size, std::vector<ground_data>( ground_size ) );
                for ( int y = 0; y < ground_size; ++y )
                {
                    for ( int x = 0; x < ground_size; ++x )
                    {
                        ground_data* data = reinterpret_cast<ground_data*>( wriable_data + index );
                        memcpy( &_ground_buffer[x][y], data, sizeof( ground_data ) );
                        index += sizeof( ground_data );
                    }
                }
            }
            else if ( name == "finished" )
            {
                _tcp_connection->on_connect_failed = nullptr;
                _tcp_connection->on_received_json = nullptr;
                _tcp_connection->on_send_failed = nullptr;
                _tcp_connection->on_disconnected = nullptr;
                _tcp_connection->on_connection = nullptr;

                if ( on_founded ) on_founded( _root_buffer, _feed_buffer, _bullet_buffer, _ground_buffer );
            }
        };
    };

    auto udp_connection = network::udp_connection::create( );
    udp_connection->set_name( "udp_connection" );
    _udp_connection = udp_connection;
    dont_destroy_node->add_child( udp_connection );

    tcp_connection->on_connect_failed = [ this ]
    {
        _is_failed = true;
    };
    tcp_connection->on_disconnected = [ this ]
    {
        _is_failed = true;
    };
    tcp_connection->on_send_failed = [ this ]
    {
        _is_failed = true;
    };
    tcp_connection->on_errored = [ this ] ( boost::system::error_code const& e )
    {
        _is_failed = true;
    };

    return true;
}
void search_room::update( float delta )
{
    if ( _is_failed )
    {
        _is_failed = false;
        scene_manager::get_instans( )->top( )->set_block_schedule_event( false );
        _tcp_connection->remove_from_parent( );
        _udp_connection->remove_from_parent( );
        if ( on_not_found ) on_not_found( );
        remove_from_parent( );
    }
}
}