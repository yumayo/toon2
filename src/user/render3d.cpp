#include "render3d.h"
#include <cinder/gl/gl.h>
using namespace cinder;
namespace user
{
CREATE_CPP( render3d )
{
    CREATE( render3d );
}
bool render3d::init( )
{
    _glsl = gl::GlslProg::create( app::loadAsset( "phone.vert" ), app::loadAsset( "phone.frag" ) );
    return true;
}
void render3d::_render( cinder::Camera const & camera, cinder::mat4 m )
{
    cinder::CameraOrtho cam;
    auto window_size_half = vec2( app::getWindowSize( ) ) / 2.0F;
    cam.setWorldUp( vec3( 0.0F, -1.0F, 0.0F ) );
    cam.lookAt( vec3( window_size_half.x, window_size_half.y, -500.0F ), vec3( window_size_half.x, window_size_half.y, 0.0F ) );
    cam.setOrtho( -window_size_half.x, window_size_half.x, -window_size_half.t, window_size_half.y, 0.125F, 999.875F );
    gl::ScopedProjectionMatrix scp_projection( cam.getProjectionMatrix( ) );
    gl::ScopedViewMatrix scp_view( cam.getViewMatrix( ) );
    gl::ScopedDepth scp_depth( true );
    gl::ScopedGlslProg scp_glsl( _glsl );
    _glsl->uniform( "uEyeDirection", normalize( vec3( 1, 1, 1 ) ) );
    node::_render( cam, m );
}
}

