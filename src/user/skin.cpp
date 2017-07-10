#include "skin.h"
#include <treelike/utility/assert_log.h>
#include "cinder/gl/gl.h"
using namespace cinder;
using namespace treelike;
namespace user
{
CREATE_CPP( skin, float radius, float segments, std::string const& relative_path )
{
    CREATE( skin, radius, segments, relative_path );
}
bool skin::init( float radius, float segments, std::string const& relative_path )
{
    if ( !renderer::circle::init( radius, segments ) ) return false;

    assert_log( !app::getAssetPath( relative_path ).empty( ), "ファイルが見つかりません。", return false );

    _texture = gl::Texture::create( loadImage( app::loadAsset( relative_path ) ) );

    _glsl = gl::getStockShader( gl::ShaderDef( ).texture( ) );

    return true;
}
void skin::render( )
{
    gl::translate( vec2( _radius ) );
    gl::scale( vec2( 1, -1 ) );

    // どう考えても重い。
    geom::Circle object;
    object.radius( _radius );
    object.subdivisions( _segments );

    _texture->bind( );
    gl::Batch::create( object, _glsl )->draw( );
}
}