#include "sprite_nearest.h"
#include "../utility/assert_log.h"
#include "cinder/gl/gl.h"
using namespace cinder;
namespace user
{
CREATE_CPP( sprite_nearest, std::string const& relative_path )
{
    CREATE( sprite_nearest, relative_path );
}
bool sprite_nearest::init( std::string const& relative_path )
{
    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );

    assert_log( !app::getAssetPath( relative_path ).empty( ), "ファイルが見つかりません。", return false );

    gl::Texture::Format format;
    format.setMinFilter( GL_NEAREST );
    format.setMagFilter( GL_NEAREST );
    _texture = gl::Texture::create( loadImage( app::loadAsset( relative_path ) ), format );
    _content_size = _texture->getSize( );

    return true;
}
void sprite_nearest::render( )
{
    gl::draw( _texture, Rectf( vec2( 0 ), _content_size ) );
}
}