#include "repeat_times.h"
namespace action
{
CREATE_CPP( repeat_times, std::shared_ptr<finite_time_action> const& time_action, int number_of_times )
{
    CREATE( repeat_times, time_action, number_of_times );
}
bool repeat_times::init( std::shared_ptr<finite_time_action> const& time_action, int number_of_times )
{
    _time_action = time_action;
    _init_number_of_times = number_of_times;
    _number_of_times = _init_number_of_times;
    return true;
}
bool repeat_times::is_done( )
{
    return _number_of_times <= 0 && _time_action->is_done( );
}
float repeat_times::update( float delta )
{
    delta = _time_action->update( delta );

    if ( _time_action->is_done( ) )
    {
        _number_of_times -= 1;
        if ( !is_done( ) )
        {
            _time_action->restart( );
            if ( delta != 0.0F )
                delta = _time_action->update( delta );
        }
    }
    return delta;
}
void repeat_times::restart( )
{
    finite_time_action::restart( );
    _number_of_times = _init_number_of_times;
    _time_action->restart( );
}
}