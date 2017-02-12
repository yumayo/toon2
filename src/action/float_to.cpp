#include "float_to.h"
using namespace cinder;
namespace action
{
CREATE_CPP( float_to, float duration, float from, float to, std::function<void( float value )> callback )
{
    CREATE( float_to, duration, from, to, callback );
}
bool float_to::init( float duration, float from, float to, float_to_callback callback )
{
    _duration = duration;
    _from = from;
    _to = to;
    _callback = callback;
    return true;
}
void float_to::step( float t )
{
    t = clamp( t, 0.0F, 1.0F );
    if ( _callback ) _callback( ease_liner( t, _from, _to ) );
}
#define l_class float_to
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( float_to
           , l_base( finite_time_action )
           , l_set( create )
    );
}
#include "lua_undef.h"
}