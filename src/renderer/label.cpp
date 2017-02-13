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
bool label::init( std::string const& text, std::string const& relative_path, float size )
{
    assert_log( !app::getAssetPath( relative_path ).empty( ), "ファイルが見つかりません。", return false );

    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );
    _relative_path = relative_path;
    _size = size;

    set_text( text );

    return true;
}
void label::render( )
{
    gl::draw( _texture, Rectf( vec2( 0 ), _content_size ) );
}
void label::set_text( std::string const & text )
{
    _layout = std::make_shared<cinder::TextLayout>( );

    // まれに例外が飛びますが、まあキャッチされているので気にしない。
    // 例外先: Windows Kits/8.1/Include/um/gdiplusfontfamily.h 143行目
    // 関数: inline BOOL FontFamily::IsStyleAvailable( IN INT style ) const
    // 内容: CacheOverflowException
    // やっぱりここやばいです。
    // Fontの中で確保したメモリは開放されない模様。
    // コメントでも、最初に作ってそれを使ってくださいって書いてある。
    _layout->setFont( Font( app::loadAsset( _relative_path ), _size ) );
    _layout->setColor( Color( 1, 1, 1 ) );
    _layout->append( text );

    bool use_alpha = true;
    _texture = gl::Texture2d::create( _layout->render( use_alpha ) );
    _content_size = _texture->getSize( );
}
#define l_class label
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( label
           , l_base( node )
           , l_set( create )
           , l_set( set_text )
    );
}
#include "lua_undef.h"
}