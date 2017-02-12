#include "label.h"
#include "../utility/assert_log.h"
#include "cinder/gl/gl.h"
#include "../utility/utf8.h"
using namespace cinder;
namespace renderer
{
CREATE_CPP( label, std::string const& text, std::string const& relative_path, float size )
{
    CREATE( label, text, relative_path, size );
}
bool label::init( )
{
    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );

    return true;
}
bool label::init( std::string const& text, std::string const& relative_path, float size )
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
void label::render( )
{
    gl::draw( _texture, Rectf( vec2( 0 ), _content_size ) );
}
#define l_class label
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( label
           , l_base( node )
           , l_set( create )
    );
}
#include "lua_undef.h"
}