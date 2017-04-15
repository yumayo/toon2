#include "letter.h"
#include "utility/assert_log.h"
#include "cinder/gl/gl.h"
using namespace cinder;
namespace renderer
{
CREATE_CPP( letter, std::string const& text, std::string const& relative_path, float size )
{
    CREATE( letter, text, relative_path, size );
}
bool letter::init( )
{
    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );

    return true;
}
bool letter::init( std::string const& text, std::string const& relative_path, float size )
{
    if ( !init( ) ) return false;

    assert_log( !app::getAssetPath( relative_path ).empty( ), "ファイルが見つかりません。", return false );

    _layout.setFont( Font( app::loadAsset( relative_path ), size ) );
    _layout.setColor( Color( 1, 1, 1 ) );
    _layout.append( text );

    bool use_alpha = true;
    _texture = gl::Texture2d::create( _layout.render( use_alpha ) );
    _content_size = _texture->getSize( );

    return true;
}
void letter::render( )
{
    gl::draw( _texture, Rectf( vec2( 0 ), _content_size ) );
}
}