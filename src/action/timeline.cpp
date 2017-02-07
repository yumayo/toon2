#include "timeline.h"
namespace action
{
CREATE_CPP( timeline )
{
    CREATE( timeline );
}
bool timeline::init( )
{
    return true;
}

#define l_class timeline
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( timeline
           , l_base( finite_time_action )
           , l_set( create )
    );
}
#include "lua_undef.h"
}