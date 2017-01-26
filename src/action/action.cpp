﻿#include "action.h"
#include "../node.h"
namespace action
{
CREATE_CPP( action )
{
    CREATE( action );
}
bool action::init( )
{
    return true;
}
void action::update( float delta )
{
    _time += delta;
    step( _time / _duration );
}
bool action::is_done( )
{
    return _duration <= _time;
}
float action::ease_liner( float t, float from, float to )
{
    return ( to - from ) * t + from;
}
void action::setup( )
{
}
void action::step( float t )
{
}



void action::stop( )
{
    _target.reset( );
}

void action::set_target( node_weak const & value )
{
    _target = value;
}

node_weak action::get_target( )
{
    return _target;
}

void action::set_tag( int value )
{
    _tag = value;
}

int action::get_tag( )
{
    return _tag;
}

void action::set_name( std::string const & value )
{
    _name = value;

    std::hash<std::string> h;
    _hash = h( value );
}

std::string action::get_name( )
{
    return _name;
}

void action::set_pause( bool value )
{
    _pause = value;
}

bool action::get_pause( )
{
    return _pause;
}
}