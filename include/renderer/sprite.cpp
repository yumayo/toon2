#include "sprite.h"
#include "utility/assert_log.h"
#include "cinder/gl/gl.h"
using namespace cinder;
namespace renderer
{
CREATE_CPP( sprite, std::string const& relative_path )
{
    CREATE( sprite, relative_path );
}
bool sprite::init( std::string const& relative_path )
{
    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );

    assert_log( !app::getAssetPath( relative_path ).empty( ), "ファイルが見つかりません。", return false );

    _texture = gl::Texture::create( loadImage( app::loadAsset( relative_path ) ) );
    _content_size = _texture->getSize( );

    return true;
}
void sprite::render( )
{
    gl::draw( _texture, Rectf( vec2( 0 ), _content_size ) );
}
}