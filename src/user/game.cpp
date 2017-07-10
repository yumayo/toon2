#include "game.h"
#include "cell_manager.h"
#include "create_dot_obeject.h"
#include <treelike/scene_manager.h>
#include "title.h"
#include <treelike/network.hpp>
#include "score_board.h"
#include "se.h"
using namespace cinder;
using namespace treelike;
namespace user
{
CREATE_CPP( game,
            Json::Value& root,
            std::vector<feed_data> feed_buffet,
            Json::Value& bullet_buffer,
            std::vector<std::vector<ground_data>>& ground_buffer )
{
    CREATE( game, root, feed_buffet, bullet_buffer, ground_buffer );
}
game::~game( )
{
    auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );
    dont_destroy_node->remove_child_by_name( "udp_connection" );
    dont_destroy_node->remove_child_by_name( "tcp_connection" );
}
bool game::init( Json::Value& root,
                 std::vector<feed_data> feed_buffet,
                 Json::Value& bullet_buffer,
                 std::vector<std::vector<ground_data>>& ground_buffer )
{
    set_schedule_update( );

    auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );

    auto tcp_connection = dont_destroy_node->get_child_by_name( "tcp_connection" ).dynamicptr<network::tcp_client>( );

    tcp_connection->on_errored = [ this ] ( boost::system::error_code const& e )
    {
        scene_manager::get_instans( )->replace( title::create( ) );
    };
    tcp_connection->on_disconnected = [ this ]
    {
        scene_manager::get_instans( )->replace( title::create( ) );
    };

    if ( auto field = field::create( root, feed_buffet, bullet_buffer, ground_buffer ) )
    {
        _field = field;
        add_child( field );
    }

    auto m = _field->get_cell_manager( ).dynamicptr<cell_manager>( );
    _player = m->get_player( );
    _ground = _field->get_ground( ).dynamicptr<ground>( );
    if ( auto controller = controller::create( _player, _ground, _field->get_bullet_manager( ).dynamicptr<bullet_manager>( ) ) )
    {
        _controller = controller;
        add_child( controller );
    }

    auto bac = dot_button::create( "back.png", 64 );
    _back_button = bac;
    bac->set_position( vec2( 64 ) + vec2( 10 ) );
    bac->on_ended = [ this ]
    {
        play_se( "sound/back.wav" );
        scene_manager::get_instans( )->replace( title::create( ) );
    };
    add_child( bac );

    add_child( score_board::create( _field->get_cell_manager( ), vec2( 300, 300 ) ) );

    return true;
}
void game::update( float delta )
{
    // カメラのアップデート

    auto max_radius = user_default::get_instans( )->get_root( )["system"]["max_radius"].asFloat( );
    auto min_radius = user_default::get_instans( )->get_root( )["system"]["min_radius"].asFloat( );
    // 0~
    auto radius = _player->get_radius( ) - min_radius;

    radius = glm::clamp( radius, 0.0F, max_radius );

    // 0.0F ~ 1.0F;
    float r = radius / max_radius;

    auto scale = 1.0F + 1.0F - easeOutCirc( r ) * 1.5F;

    _field->set_scale( vec2( scale ) );

    auto win_half = vec2( app::getWindowSize( ) ) * 0.5F;
    if ( _player ) _field->set_position( win_half - _player->get_position( ) * _field->get_scale( ).x );
}
}