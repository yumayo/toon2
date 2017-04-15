#include "repeat_forever.h"
namespace action
{
CREATE_CPP( repeat_forever, std::shared_ptr<finite_time_action> const& time_action )
{
    CREATE( repeat_forever, time_action );
}
bool repeat_forever::init( std::shared_ptr<finite_time_action> const& time_action )
{
    _time_action = time_action;
    return true;
}
void repeat_forever::setup( )
{
    _time_action->setup( _target.lock( ), _pause );
}
bool repeat_forever::is_done( )
{
    // 無限ループを作成するクラスなので。
    return false;
}
float repeat_forever::update( float delta )
{
    delta = _time_action->update( delta );
    if ( _time_action->is_done( ) )
    {
        _time_action->restart( );
        if ( delta != 0.0F ) 
            delta = _time_action->update( delta );
    }
    return delta;
}
void repeat_forever::restart( )
{
    finite_time_action::restart( );
    _time_action->restart( );
}
}