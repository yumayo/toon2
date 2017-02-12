#include "repeat_times.h"
namespace action
{
CREATE_CPP( repeat_times, std::shared_ptr<finite_time_action> time_action, int number_of_times )
{
    CREATE( repeat_times, time_action, number_of_times );
}
bool repeat_times::init( std::shared_ptr<finite_time_action> time_action, int number_of_times )
{
    _time_action = time_action;
    _number_of_times = number_of_times;
    return true;
}
bool repeat_times::is_done( )
{
    return _number_of_times <= 0 && _time_action->is_done( );
}
void repeat_times::update( float delta )
{
    _time_action->update( delta );

    if ( _time_action->is_done( ) )
    {
        _number_of_times -= 1;
        if ( !is_done( ) )
        {
            _time_action->restart( );
        }
    }
}
#define l_class repeat_times
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( repeat_times,
           l_base( finite_time_action ),
           l_set( create )
    );
}
#include "lua_undef.h"
}