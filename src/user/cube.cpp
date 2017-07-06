#include "cube.h"
#include "cinder/gl/gl.h"
using namespace cinder;
using namespace treelike;
namespace user
{
CREATE_CPP( cube, cinder::vec3 const & size )
{
    CREATE( cube, size );
}
bool cube::init( cinder::vec3 const& size )
{
    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );
    _content_size = vec2( size.x, size.y );
    _size = size;
    _shader = gl::GlslProg::create( app::loadAsset( "phone.vert" ), app::loadAsset( "phone.frag" ) );
    return true;
}
void cube::render( )
{
    gl::ScopedGlslProg glslScp( _shader );
    _shader->uniform( "uEyeDirection", normalize( vec3( 0.5F, 0.5F, 1 ) ) );
    gl::enableDepth( true );
    gl::pushModelView( );
    gl::translate( _size / 2.0F );
    gl::drawCube( vec3( 0 ), _size );
    gl::popModelView( );
    gl::enableDepth( false );
}
}