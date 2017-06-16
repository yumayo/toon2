#include "feed_manager.h"
#include "feed.h"
#include "player_manager.h"
#include "scene_manager.h"
#include "user_default.h"
#include "utility.hpp"
using namespace cinder;
namespace user
{
CREATE_CPP( feed_manager, std::weak_ptr<node> player_manager, Json::Value const& feed_root )
{
    CREATE( feed_manager, player_manager, feed_root );
}
bool feed_manager::init( std::weak_ptr<node> player_manager, Json::Value const& feed_root )
{
    set_name( "feed_manager" );

    _player_manager = player_manager;

    auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );
    _tcp_connection = std::dynamic_pointer_cast<network::tcp_client>( dont_destroy_node.lock( )->get_child_by_name( "tcp_connection" ) );

    for ( auto& f : feed_root )
    {
        add_child( feed::create( f["tag"].asInt( ), cinder::vec2( f["position"][0].asInt( ), f["position"][1].asInt( ) ) ) );
    }

    _tcp_connection.lock( )->on_received_named_json.insert( std::make_pair( "feed_captured", [ this ] ( Json::Value root )
    {
        remove_child_by_tag( root["data"]["erase_tag"].asInt( ) );
        add_child( feed::create( root["data"]["tag"].asInt( ), cinder::vec2( root["data"]["position"][0].asInt( ), root["data"]["position"][1].asInt( ) ) ) );
    } ) );

    set_schedule_update( );

    return true;
}
void feed_manager::update( float delta )
{
    auto pla_manager = std::dynamic_pointer_cast<player_manager>( _player_manager.lock( ) );
    auto player = pla_manager->get_player( );
    for ( auto& f : _children )
    {
        auto fee = std::dynamic_pointer_cast<feed>( f );
        if ( fee->is_captureing( ) ) continue;

        // 自分の半径の二倍分の距離から吸い取れます。
        if ( distance( fee->get_position( ), player.lock( )->get_position( ) )
             < fee->get_radius( ) * 2 + player.lock( )->get_radius( ) )
        {
            fee->captured( player.lock( ) );
        }
    }
}
void feed_manager::on_feed_captured( Json::Value const& root )
{
    utility::log( "[%d]on_feed_captured", root["data"]["tag"].asInt( ) );
    _tcp_connection.lock( )->write( Json::FastWriter( ).write( root ) );
}
}

