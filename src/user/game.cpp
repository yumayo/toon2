﻿#include "game.h"
#include "player_manager.h"
#include "create_dot_obeject.h"
#include "scene_manager.h"
#include "title.h"
#include "network.hpp"
#include "score_board.h"
using namespace cinder;
namespace user
{
CREATE_CPP( game, Json::Value& root, std::map<int, cinder::ivec2>& feeds_buffer, std::vector<std::vector<unsigned char>>& ground_buffer )
{
    CREATE( game, root, feeds_buffer, ground_buffer );
}
game::~game( )
{
    auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );
    dont_destroy_node.lock( )->remove_child_by_name( "udp_connection" );
    dont_destroy_node.lock( )->remove_child_by_name( "tcp_connection" );
}
bool game::init( Json::Value& root, std::map<int, cinder::ivec2>& feeds_buffer, std::vector<std::vector<unsigned char>>& ground_buffer )
{
    set_schedule_update( );

    auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );

    auto tcp_connection = std::dynamic_pointer_cast<network::tcp_client>( dont_destroy_node.lock( )->get_child_by_name( "tcp_connection" ) );

    tcp_connection->on_errored = [ this ] ( asio::error_code const& e )
    {
        scene_manager::get_instans( )->replace( title::create( ) );
    };
    tcp_connection->on_disconnected = [ this ]
    {
        scene_manager::get_instans( )->replace( title::create( ) );
    };

    if ( auto field = field::create( root, feeds_buffer, ground_buffer ) )
    {
        _field = field;
        add_child( field );
    }

    _player = std::dynamic_pointer_cast<player_manager>( _field.lock( )->get_child_by_name( "player_manager" ) )->get_player( );
    _ground = std::dynamic_pointer_cast<ground>( _field.lock( )->get_child_by_name( "ground" ) );
    if ( auto controller = controller::create( _player, _ground ) )
    {
        _controller = controller;
        add_child( controller );
    }

    auto bac = create_dot_button( "back.png", 64 );
    {
        auto sound = ::audio::buffer_player::create( "sound/back.wav" );
        sound->set_name( "sound" );
        bac->add_child( sound );
    }
    _back_button = bac;
    bac->set_position( vec2( 64 ) + vec2( 10 ) );
    bac->on_ended = [ this ]
    {
        std::dynamic_pointer_cast<::audio::buffer_player> ( _back_button.lock( )->get_child_by_name( "sound" ) )->play( );

        scene_manager::get_instans( )->replace( title::create( ) );
    };
    add_child( bac );

    add_child( score_board::create( _field.lock( )->get_child_by_name( "player_manager" ), vec2( 300, 300 ) ) );

    auto udp_connection = dont_destroy_node.lock( )->get_child_by_name( "udp_connection" );
    tcp_connection->set_schedule_update( );
    udp_connection->set_schedule_update( );

    return true;
}
void game::update( float delta )
{
    // カメラのアップデート

    // 0~
    auto radius = _player.lock( )->get_radius( ) - _player.lock( )->get_startup_radius( );

    // ~300
    radius = glm::clamp( radius, 0.0F, 300.0F );

    // 0.0F ~ 1.0F;
    float r = radius / 300.0F;

    auto scale = 1.0F + 1.0F - easeOutCirc( r ) * 1.5F;

    _field.lock( )->set_scale( vec2( scale ) );

    auto win_half = vec2( app::getWindowSize( ) ) * 0.5F;
    if ( _player.lock( ) ) _field.lock( )->set_position( win_half - _player.lock( )->get_position( ) * _field.lock( )->get_scale( ).x );
}
}