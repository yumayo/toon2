#include "spawn.h"
namespace action
{
CREATE_CPP( spawn, sol::variadic_args const& args )
{
    CREATE( spawn, args );
}
bool spawn::init( )
{
    return true;
}
bool spawn::init( sol::variadic_args const & args )
{
    for ( auto& act : args )
    {
        std::shared_ptr<finite_time_action> sa = act;
        _actions.emplace_back( std::move( sa ) );
    }
    return init( );
}
void spawn::setup( )
{
    for ( auto& act : _actions )
    {
        act->setup( _target, _pause );
    }
}
bool spawn::is_done( )
{
    bool done = true;
    for ( auto const& act : _actions )
    {
        done = done && act->is_done( );
    }
    return done;
}
void spawn::update( float delta )
{
    for ( auto const& act : _actions )
    {
        act->update( delta );
    }
}
void spawn::restart( )
{
    timeline::restart( );
    if ( is_done( ) )
    {
        for ( auto const& act : _actions )
        {
            act->restart( );
        }
        init( );
        setup( );
    }
}
#define l_class spawn
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( spawn,
           l_base( timeline ),
           "create", [ ] ( sol::variadic_args const& args ) { return spawn::create( args ); }
    );
}
#include "lua_undef.h"
}