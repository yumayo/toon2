#include "sequence.h"
namespace action
{
CREATE_CPP( sequence, sol::variadic_args const& args )
{
    CREATE( sequence, args );
}

bool sequence::init( )
{
    _target_action = _actions.begin( );
    return true;
}
void sequence::setup( )
{
    if ( is_done( ) ) return;
    ( *_target_action )->setup( _target.lock( ), _pause );
}
bool sequence::is_done( )
{
    return _target_action == _actions.end( );
}
void sequence::update( float delta )
{
    if ( is_done( ) ) return;

    ( *_target_action )->update( delta );
    if ( ( *_target_action )->is_done( ) )
    {
        _target_action++;

        setup( );
    }
}
void sequence::restart( )
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
bool sequence::init( sol::variadic_args const& args )
{
    for ( auto& act : args )
    {
        std::shared_ptr<finite_time_action> sa = act;
        _actions.emplace_back( std::move( sa ) );
    }
    return init( );
}
#define l_class sequence
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( sequence,
           l_base( timeline ),
           "create", [ ] ( sol::variadic_args const& args ) { return sequence::create( args ); }
    );
}
#include "lua_undef.h"
}