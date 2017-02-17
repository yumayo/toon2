#include "finite_time_action.h"
#include "../node.h"
namespace action
{
CREATE_CPP( finite_time_action, float duration )
{
    CREATE( finite_time_action, duration );
}
bool finite_time_action::init( float duration )
{
    _duration = duration;
    return true;
}
void finite_time_action::update( float delta )
{
    _time += delta;
    step( _time / _duration );
}
void finite_time_action::restart( )
{
    _time = 0.0F;
}
void finite_time_action::set_time( float value )
{
    _time = value;
}
float finite_time_action::get_time( )
{
    return _time;
}
void finite_time_action::set_duration( float value )
{
    _duration = value;
}
float finite_time_action::get_duration( )
{
    return _duration;
}
bool finite_time_action::is_done( )
{
    return _duration <= _time;
}

#define l_class finite_time_action
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( finite_time_action
           , l_base( action )
           , l_set( create )
           , l_prop( time )
           , l_prop( duration )
    );
}
#include "lua_undef.h"
}