﻿#include "finite_time_action.h"
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
void finite_time_action::set_time( float value )
{
    _time = value;
}
float finite_time_action::get_time( )
{
    return _time;
}
void finite_time_action::set_duration( float value )
{
    _duration = value;
}
float finite_time_action::get_duration( )
{
    return _duration;
}
bool finite_time_action::is_done( )
{
    return _duration <= _time;
}
}