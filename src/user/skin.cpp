#include "skin.h"
#include "utility/assert_log.h"
#include "cinder/gl/gl.h"

using namespace cinder;
namespace user
{
CREATE_CPP( skin, float radius, std::string const& relative_path )
{
    CREATE( skin, radius, relative_path );
}
bool skin::init( float radius, std::string const& relative_path )
{
    if ( !renderer::circle::init( radius ) ) return false;

    assert_log( !app::getAssetPath( relative_path ).empty( ), "ファイルが見つかりません。", return false );

    _texture = gl::Texture::create( loadImage( app::loadAsset( relative_path ) ) );

    // 円依存なので
    // _content_size = _texture->getSize( );

    _glsl = gl::getStockShader( gl::ShaderDef( ).texture( ).lambert( ) );

    return true;
}
void skin::render( )
{
    gl::translate( vec2( _radius ) );
    gl::scale( vec2( 1, -1 ) );

    // どう考えても重い。
    geom::Circle object;
    object.radius( _radius );
    object.subdivisions( _radius );

    _texture->bind( );
    gl::Batch::create( object, _glsl )->draw( );
}
}