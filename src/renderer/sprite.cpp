#include "sprite.h"
#include "cinder/gl/gl.h"
using namespace cinder;
namespace renderer
{
CREATE_CPP( sprite )
{
    CREATE( sprite );
}
CREATE_CPP( sprite, std::string const& relative_path )
{
    CREATE( sprite, relative_path );
}
bool sprite::init( )
{
    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );

    return true;
}
bool sprite::init( std::string const& relative_path )
{
    if ( !init( ) ) return false;

    assert_log( !app::getAssetPath( relative_path ).empty( ), "ファイルが見つかりません。" );

    _texture = gl::Texture::create( loadImage( app::loadAsset( relative_path ) ) );
    _content_size = _texture->getSize( );

    return true;
}
void sprite::render( )
{
    gl::draw( _texture, Rectf( vec2( 0 ), _content_size ) );
}
}