#include "dot_label.h"
#include "../utility/assert_log.h"
#include "cinder/gl/gl.h"
#include "../utility/utf8.h"
using namespace cinder;
namespace user
{
CREATE_CPP( dot_label, std::string const& text, std::string const& relative_path, float size )
{
    CREATE( dot_label, text, relative_path, size );
}
bool dot_label::init( std::string const& text, std::string const& relative_path, float size )
{
    assert_log( !app::getAssetPath( relative_path ).empty( ), "ファイルが見つかりません。", return false );

    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );
    _relative_path = relative_path;
    _size = size;

    // まれに例外が飛びますが、まあキャッチされているので気にしない。
    // 例外先: Windows Kits/8.1/Include/um/gdiplusfontfamily.h 143行目
    // 関数: inline BOOL FontFamily::IsStyleAvailable( IN INT style ) const
    // 内容: CacheOverflowException
    // やっぱりここやばいです。
    // Fontの中で確保したメモリは開放されない模様。
    // コメントでも、最初に作ってそれを使ってくださいって書いてある。
    _font = Font( app::loadAsset( _relative_path ), _size );

    set_text( text );

    return true;
}
void dot_label::render( )
{
    gl::draw( _texture, Rectf( vec2( 0 ), _content_size ) );
}
void dot_label::set_text( std::string const & text )
{
    _layout = std::make_shared<cinder::TextLayout>( );
    _layout->setFont( _font );
    _layout->setColor( Color( 1, 1, 1 ) );
    _layout->append( text );

    gl::Texture::Format format;
    format.setMinFilter( GL_NEAREST );
    format.setMagFilter( GL_NEAREST );
    bool use_alpha = true;
    _texture = gl::Texture::create( _layout->render( use_alpha ), format );
    _content_size = _texture->getSize( );
}
}