﻿#include "spawn.h"
namespace action
{
bool spawn::init( )
{
    return true;
}
void spawn::setup( )
{
    for ( auto& act : _actions )
    {
        act->setup( _target.lock( ), _pause );
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
}