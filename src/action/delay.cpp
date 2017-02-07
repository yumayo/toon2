#include "finite_time_action.h"
#include "../node.h"
#include "delay.h"
namespace action
{
CREATE_CPP( delay, float duration )
{
    CREATE( delay, duration );
}
bool delay::init( float duration )
{
    _duration = duration;
    return true;
}
#define l_class delay
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( delay
           , l_base( finite_time_action )
           , l_set( create )
    );
}
#include "lua_undef.h"
}