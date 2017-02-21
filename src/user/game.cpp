#include "game.h"
#include "player_manager.h"
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

    auto pla_mgr = std::dynamic_pointer_cast<player_manager>( _field.lock( )->get_child_by_name( "player_manager" ) );
    _own = pla_mgr->get_player( );

    if ( auto controller = controller::create( _own ) )
    {
        _controller = controller;
        add_child( controller );
    }
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