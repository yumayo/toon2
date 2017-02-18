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
void repeat_forever::update( float delta )
{
    _time_action->update( delta );

    if ( _time_action->is_done( ) )
    {
        _time_action->restart( );
    }
}
void repeat_forever::restart( )
{
    finite_time_action::restart( );
    _time_action->restart( );
}
#define l_class repeat_forever
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( repeat_forever,
           l_base( finite_time_action ),
           l_set( create )
    );
}
#include "lua_undef.h"
}