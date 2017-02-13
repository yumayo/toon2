﻿#include "finite_time_action.h"
#include "../node.h"
#include "call_func.h"
namespace action
{
CREATE_CPP( call_func, std::function<void( )> call )
{
    CREATE( call_func, call );
}
bool call_func::init( std::function<void( )> call )
{
    _call = call;
    return true;
}
void call_func::update( float delta )
{
    if ( _call ) _call( );
}
#define l_class call_func
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( call_func
           , l_base( finite_time_action )
           , l_set( create )
    );
}
#include "lua_undef.h"
}