﻿#include "search_room.h"
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

    auto client = network::udp_connection::create( );
    _client = client;
    add_child( client );
    client->set_name( "connection" );
    client->on_receive_failed = [ this ]
    {
        scene_manager::get_instans( )->top( )->set_block_schedule_event( false );
        if ( on_not_found ) on_not_found( );

        remove_from_parent( );
    };
    client->on_received_json = [ this ] ( network::network_handle handle, Json::Value root )
    {
        if ( root["name"].asString( ) != "founded" ) return;

        remove_action_by_name( "find" );
        auto node = scene_manager::get_instans( )->get_dont_destroy_node( );
        _client.lock( )->set_parent( node.lock( ) );
        _client.lock( )->on_receive_failed = nullptr;
        _client.lock( )->on_received_json = nullptr;
        _client.lock( )->set_schedule_update( false );
        if ( on_founded ) on_founded( root );
    };

    // 0.2秒ごとにデータを送り、3回とも返答がなかった場合はfailedを呼びます。
    auto find_server = action::sequence::create( action::repeat_times::create( action::sequence::create( action::call_func::create( [ this ]
    {
        Json::Value root;
        root["name"] = "find_room";
        root["data"]["select_skin_name"] = user_default::get_instans( )->get_root( )["select_skin_name"].asString( );
        auto address = user_default::get_instans( )->get_root( )["server"]["address"].asString( );
        auto port = user_default::get_instans( )->get_root( )["server"]["port"].asInt( );
        _client.lock( )->write( std::make_shared<network::network_object>( address, port ), Json::FastWriter( ).write( root ) );
    } ), action::delay::create( 0.2F ) ), 3 ), action::call_func::create( [ this ]
    {
        _client.lock( )->on_receive_failed( );
    } ) );
    find_server->set_name( "find" );
    run_action( find_server );

    return true;
}
}