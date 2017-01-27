#include "finite_time_action.h"
#include "../node.h"
namespace action
{
CREATE_CPP( finite_time_action )
{
    CREATE( finite_time_action );
}
bool finite_time_action::init( )
{
    return true;
}
void finite_time_action::update( float delta )
{
    _time += delta;
    step( _time / _duration );
}
bool finite_time_action::is_done( )
{
    return _duration <= _time;
}
}