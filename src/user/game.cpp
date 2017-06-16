#include "game.h"
#include "player_manager.h"
#include "create_dot_obeject.h"
#include "scene_manager.h"
#include "title.h"
using namespace cinder;
namespace user
{
CREATE_CPP( game, Json::Value root )
{
    CREATE( game, root );
}
bool game::init( Json::Value& root )
{
    set_schedule_update( );

    if ( auto field = field::create( root ) )
    {
        _field = field;
        add_child( field );
    }

    auto pla_mgr = _field.lock( )->get_child_by_name( "player_manager" );
    _own = std::dynamic_pointer_cast<player>( pla_mgr->get_child_by_name( "own" ) );
    _ground = std::dynamic_pointer_cast<ground>( _field.lock( )->get_child_by_name( "ground" ) );

    if ( auto controller = controller::create( _own, _ground ) )
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

        auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );
        dont_destroy_node.lock( )->remove_child_by_name( "udp_connection" );
        dont_destroy_node.lock( )->remove_child_by_name( "tcp_connection" );

        scene_manager::get_instans( )->replace( title::create( ) );
    };
    add_child( bac );

    return true;
}
void game::update( float delta )
{
    // カメラのアップデート
    auto win_half = vec2( app::getWindowSize( ) ) * 0.5F;
    if ( _own.lock( ) )
        _field.lock( )->set_position( win_half - _own.lock( )->get_position( ) );
}
}