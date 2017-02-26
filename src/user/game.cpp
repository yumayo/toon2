#include "game.h"
#include "player_manager.h"
#include "create_dot_obeject.h"
#include "../scene_manager.h"
#include "title.h"
using namespace cinder;
namespace user
{
CREATE_CPP( game )
{
    CREATE( game );
}
bool game::init( )
{
    set_schedule_update( );

    if ( auto field = field::create( ) )
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
    bac->set_position( vec2( 64 ) + vec2( 10 ) );
    bac->on_ended = [ ]
    {
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