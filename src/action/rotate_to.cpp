#include "rotate_to.h"
#include "../node.h"
using namespace cinder;
namespace action
{
CREATE_CPP( rotate_to, float duration, float rotation )
{
    CREATE( rotate_to, duration, rotation );
}
bool rotate_to::init( float duration, float rotation )
{
    _duration = duration;
    _rotation = rotation;
    return true;
}
void rotate_to::setup( )
{
    _start_rotation = _target.lock( )->get_rotation( );
}
void rotate_to::step( float t )
{
    t = clamp( t, 0.0F, 1.0F );

    auto const to = _rotation;
    auto const from = _start_rotation;
    auto const temp = ease_liner( t, from, to );
    _target.lock( )->set_rotation( temp );
}

#define l_class rotate_to
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( rotate_to
           , l_base( finite_time_action )
           , l_set( create )
    );
}
#include "lua_undef.h"
}