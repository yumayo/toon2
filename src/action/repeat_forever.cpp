#include "repeat_forever.h"
namespace action
{
CREATE_CPP( repeat_forever, std::weak_ptr<finite_time_action> time_action )
{
    CREATE( repeat_forever, time_action );
}
bool repeat_forever::init( std::weak_ptr<finite_time_action> time_action )
{
    _time_action = time_action;
    return true;
}
bool repeat_forever::is_done( )
{
    // 無限ループを作成するクラスなので。
    return false;
}
void repeat_forever::update( float delta )
{
    if ( _time_action.lock( )->is_done( ) )
    {
    }
}
}