#include "toon2.h"
#include "cinder/gl/gl.h"
#include "jsoncpp/json.h"
using namespace cinder;
bool toon2::init( )
{
    Json::Value root;
    Json::Reader reader;

    auto loadString = [ this ] ( std::string const& relative_path )
    {
        return std::string( static_cast<char*>( app::loadAsset( relative_path )->getBuffer( )->getData( ) ) );
    };

    if ( reader.parse( loadString( u8"audio.json" ), root ) )
    {
        auto str = root[u8"BGM"][u8"city"].asString( );
    }

    camera.setPerspective( 60.0F, app::getWindowAspectRatio( ), 0.1F, 100.0F );
    camera.lookAt( vec3( 0, 0, -5 ), vec3( 0 ) );

    gl::enableDepth( );

    set_schedule_update( );

    return true;
}
void toon2::update( float delta )
{
    rotate += delta;
}
void toon2::draw( )
{
    gl::clear( );

    gl::setMatrices( camera );

    gl::pushModelView( );
    gl::rotate( rotate, vec3( 1, 0, 0 ) );
    gl::rotate( rotate, vec3( 0, 1, 0 ) );
    gl::rotate( rotate, vec3( 0, 0, 1 ) );
    gl::drawColorCube( vec3( 0 ), vec3( 1 ) );
    gl::popModelView( );
}