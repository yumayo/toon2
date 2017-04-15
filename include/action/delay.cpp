#include "finite_time_action.h"
#include "node.h"
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
}