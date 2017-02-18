#include "game.h"
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
        if ( auto player = player::create( ) )
        {
            _player = player;
            field->add_child( player );
        }
    }
    if ( auto controller = controller::create( _player ) )
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
    _field.lock( )->set_position( win_half - _player.lock( )->get_position( ) );
}
}