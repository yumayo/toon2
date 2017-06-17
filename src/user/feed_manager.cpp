﻿#include "feed_manager.h"
#include "feed.h"
#include "player_manager.h"
#include "scene_manager.h"
#include "user_default.h"
#include "utility.hpp"
#include "action.hpp"
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
        for ( auto& obj : root["data"] )
        {
            remove_child_by_tag( obj["erase_tag"].asInt( ) );
            add_child( feed::create( obj["tag"].asInt( ), cinder::vec2( obj["position"][0].asInt( ), obj["position"][1].asInt( ) ) ) );
        }
    } ) );

    // エサは0.2秒ごとに同期を取ります。
    // 短い時間でwriteするとどこかで情報をロスします。
    // tcpでもそんなことあるんやな。
    // 送信エラーとかにもならないし。
    run_action( action::repeat_forever::create( action::sequence::create( action::delay::create( 0.2F ), action::call_func::create( [ this ] 
    {
        if ( _captured_feed_number != 0 )
        {
            _captured_feed_data["name"] = "feed_captured";
            _tcp_connection.lock( )->write( Json::FastWriter( ).write( _captured_feed_data ) );

            _captured_feed_number = 0;
            _captured_feed_data.clear( );
        }
    } ) ) ) );

    set_schedule_update( );

    return true;
}
void feed_manager::update( float delta )
{
    for ( auto child : _player_manager.lock( )->get_children( ) )
    {
        if ( auto pla = std::dynamic_pointer_cast<player>( child ) )
        {
            for ( auto& f : _children )
            {
                auto fee = std::dynamic_pointer_cast<feed>( f );
                if ( fee->is_captureing( ) ) continue;

                // 自分の半径の二倍分の距離から吸い取れます。
                if ( distance( fee->get_position( ), pla->get_position( ) )
                     < fee->get_radius( ) * 2 + pla->get_radius( ) )
                {
                    fee->captured( pla );
                }
            }
        }
    }
}
void feed_manager::on_feed_captured( int tag )
{
    utility::log( "[%d]on_feed_captured", tag );
    _captured_feed_data["data"][_captured_feed_number++]["tag"] = tag;
}
}

