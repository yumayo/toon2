﻿#include "circle.h"
#include "cinder/gl/gl.h"
using namespace cinder;
namespace renderer
{
CREATE_CPP( circle, float radius, float segments )
{
    CREATE( circle, radius, segments );
}
bool circle::init( float radius, float segments )
{
    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );
    set_radius( radius );
    _segments = segments;
    return true;
}
void circle::render( )
{
    gl::drawSolidCircle( vec2( _radius, _radius ), _radius, _segments );
}
void circle::set_radius( float value )
{
    _radius = value;
    _content_size = { _radius * 2, _radius * 2 };
}
float circle::get_radius( )
{
    return _radius;
}
#define l_class circle
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( circle
           , l_base( node )
           , l_set( create )
    );
}
#include "lua_undef.h"
}