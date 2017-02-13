﻿#include "move_by.h"
#include "../node.h"
using namespace cinder;
namespace action
{
CREATE_CPP( move_by, float duration, cinder::vec2 position )
{
    CREATE( move_by, duration, position );
}
bool move_by::init( float duration, cinder::vec2 position )
{
    _duration = duration;
    _init_position = position;
    return true;
}
void move_by::setup( )
{
    _start_position = _target.lock( )->get_position( );
    _position = _init_position + _start_position;
}

#define l_class move_by
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( move_by
           , l_base( finite_time_action )
           , l_set( create )
    );
}
#include "lua_undef.h"
}