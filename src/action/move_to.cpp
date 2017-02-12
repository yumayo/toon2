#include "move_to.h"
#include "../node.h"
using namespace cinder;
namespace action
{
CREATE_CPP( move_to, float duration, cinder::vec2 position )
{
    CREATE( move_to, duration, position );
}
bool move_to::init( float duration, cinder::vec2 position )
{
    _duration = duration;
    _position = position;
    return true;
}
void move_to::setup( )
{
    _start_position = _target.lock( )->get_position( );
}
void move_to::step( float t )
{
    t = clamp( t, 0.0F, 1.0F );

    auto const to = _position;
    auto const from = _start_position;
    auto const temp = vec2( ease_liner( t, from.x, to.x ), ease_liner( t, from.y, to.y ) );
    _target.lock( )->set_position( temp );
}

#define l_class move_to
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( move_to
           , l_base( finite_time_action )
           , l_set( create )
    );
}
#include "lua_undef.h"
}