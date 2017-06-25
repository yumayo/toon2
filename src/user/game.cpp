#include "game.h"
#include "cell_manager.h"
#include "create_dot_obeject.h"
#include "scene_manager.h"
#include "title.h"
#include "network.hpp"
#include "score_board.h"
#include "se.h"
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

    _player = std::dynamic_pointer_cast<cell_manager>( _field.lock( )->get_child_by_name( "cell_manager" ) )->get_player( );
    _ground = std::dynamic_pointer_cast<ground>( _field.lock( )->get_child_by_name( "ground" ) );
    if ( auto controller = controller::create( _player, _ground ) )
    {
        _controller = controller;
        add_child( controller );
    }

    auto bac = create_dot_button( "back.png", 64 );
    _back_button = bac;
    bac->set_position( vec2( 64 ) + vec2( 10 ) );
    bac->on_ended = [ this ]
    {
        play_se( "sound/back.wav" );
        scene_manager::get_instans( )->replace( title::create( ) );
    };
    add_child( bac );

    add_child( score_board::create( _field.lock( )->get_child_by_name( "cell_manager" ), vec2( 300, 300 ) ) );

    return true;
}
void game::update( float delta )
{
    // カメラのアップデート

    // 0~
    auto radius = _player.lock( )->get_radius( ) - _player.lock( )->get_startup_radius( );

    radius = glm::clamp( radius, 0.0F, user_default::get_instans( )->get_root( )["system"]["player_raidus"]["max"].asFloat( ) );

    // 0.0F ~ 1.0F;
    float r = radius / user_default::get_instans( )->get_root( )["system"]["player_raidus"]["max"].asFloat( );

    auto scale = 1.0F + 1.0F - easeOutCirc( r ) * 1.5F;

    _field.lock( )->set_scale( vec2( scale ) );

    auto win_half = vec2( app::getWindowSize( ) ) * 0.5F;
    if ( _player.lock( ) ) _field.lock( )->set_position( win_half - _player.lock( )->get_position( ) * _field.lock( )->get_scale( ).x );
}
}