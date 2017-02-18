#include "rect_edge.h"
#include "cinder/gl/gl.h"
using namespace cinder;
namespace renderer
{
CREATE_CPP( rect_edge, cinder::vec2 const & size, float line_width )
{
    CREATE( rect_edge, size, line_width );
}
bool rect_edge::init( cinder::vec2 const& size, float line_width )
{
    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );
    _content_size = size;
    _line_width = line_width;
    return true;
}
void rect_edge::render( )
{
    gl::lineWidth( _line_width );
    gl::drawStrokedRect( Rectf( vec2( 0 ), _content_size ) );
    gl::lineWidth( 1.0F );
}
#define l_class rect_edge
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( rect_edge
           , l_base( node )
           , l_set( create )
    );
}
#include "lua_undef.h"
}