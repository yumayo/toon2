#include "sphere.h"
#include "cinder/gl/gl.h"
using namespace cinder;
namespace user
{
CREATE_CPP( sphere, float radius, float subdivisions )
{
    CREATE( sphere, radius, subdivisions );
}
sphere::~sphere( )
{
}
bool sphere::init( float radius, float subdivisions )
{
    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );
    set_radius( radius );
    set_subdivisions( subdivisions );
    _shader = gl::GlslProg::create( app::loadAsset( "phone.vert" ), app::loadAsset( "phone.frag" ) );
    return true;
}
void sphere::render( )
{
    gl::ScopedGlslProg glslScp( _shader );
    _shader->uniform( "uEyeDirection", normalize( vec3( 0.5F, 0.5F, 1 ) ) );
    gl::enableDepth( true );
    gl::drawSphere( vec3( _radius, _radius, 0 ), _radius, _subdivisions );
    gl::enableDepth( false );
}
void sphere::set_radius( float value )
{
    _radius = value;
    _content_size = { _radius * 2, _radius * 2 };
}
float sphere::get_radius( )
{
    return _radius;
}
void sphere::set_subdivisions( float value )
{
    _subdivisions = value;
}
float sphere::get_subdivisions( )
{
    return _subdivisions;
}
}