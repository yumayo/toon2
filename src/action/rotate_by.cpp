#include "rotate_by.h"
#include "../node.h"
using namespace cinder;
namespace action
{
CREATE_CPP( rotate_by, float duration, float rotation )
{
    CREATE( rotate_by, duration, rotation );
}
bool rotate_by::init( float duration, float rotation )
{
    _duration = duration;
    _init_rotation = rotation;
    return true;
}
void rotate_by::setup( )
{
    _start_rotation = _target.lock( )->get_rotation( );
    _rotation = _init_rotation + _start_rotation;
}

#define l_class rotate_by
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( rotate_by
           , l_base( finite_time_action )
           , l_set( create )
    );
}
#include "lua_undef.h"
}