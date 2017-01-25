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

    gl::enableDepth( );

    set_position( vec2( 100, 100 ) );


    set_content_size( vec2( 100, 100 ) );

    set_color( ColorA( 1, 1, 1, 1 ) );

    set_anchor_point( vec2( 0.5, 0.5 ) );

    set_schedule_update( );

    return true;
}
void toon2::update( float delta )
{
    set_rotation( get_rotation( ) + delta );
}
void toon2::render( )
{
    gl::drawSolidRect( Rectf( vec2( 0 ), _content_size ) );
}