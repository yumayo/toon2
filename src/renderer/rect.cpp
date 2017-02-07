﻿#include "rect.h"
#include "cinder/gl/gl.h"
using namespace cinder;
namespace renderer
{
CREATE_CPP( rect, cinder::vec2 const & size )
{
    CREATE( rect, size );
}
bool rect::init( cinder::vec2 const& size )
{
    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );
    _content_size = size;
    return true;
}
void rect::render( )
{
    gl::drawSolidRect( Rectf( vec2( 0 ), _content_size ) );
}
#define l_class rect
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( rect
           , l_base( node )
           , l_set( create )
    );
}
#include "lua_undef.h"
}