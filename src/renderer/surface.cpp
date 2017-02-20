#include "surface.h"
#include "../utility/assert_log.h"
#include "cinder/gl/gl.h"
using namespace cinder;
namespace renderer
{
CREATE_CPP( surface, cinder::vec2 size, cinder::ColorA color )
{
    CREATE( surface, size, color );
}
bool surface::init( cinder::vec2 size, cinder::ColorA color )
{
    // サーフェスは左上原点で行きます。

    _surface = Surface32f( size.x, size.y, true );
    for ( int y = 0; y < _surface.getHeight( ); ++y )
    {
        for ( int x = 0; x < _surface.getWidth( ); ++x )
        {
            _surface.setPixel( vec2( x, y ), color );
        }
    }
    _texture = gl::Texture::create( _surface );
    _content_size = _surface.getSize( );

    return true;
}
void surface::render( )
{
    _texture->update( _surface );
    gl::draw( _texture, Rectf( vec2( 0 ), _content_size ) );
}
void surface::set_pixel( cinder::vec2 pixel, cinder::ColorA color )
{
    _surface.setPixel( pixel, color );
}
cinder::ColorA surface::get_pixel( cinder::vec2 pixel )
{
    return _surface.getPixel( pixel );
}
#define l_class surface
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( surface
           , l_base( node )
           , l_set( create )
           , l_set( set_pixel )
           , l_set( get_pixel )
    );
}
#include "lua_undef.h"
}