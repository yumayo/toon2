#include "button.h"
#include "cinder/gl/gl.h"
#include "utility/collision.h"
#include "utility/assert_log.h"
using namespace cinder;
namespace renderer
{
CREATE_CPP( button, cinder::vec2 const & size )
{
    CREATE( button, size );
}
CREATE_CPP( button, std::string const & normal_texture_relative_path, std::string const & swallow_texture_relative_path )
{
    CREATE( button, normal_texture_relative_path, swallow_texture_relative_path );
}
bool button::init( cinder::vec2 const& size )
{
    set_schedule_mouse_event( );
    set_schedule_touch_event( );

    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );
    _content_size = size;

    return true;
}
bool button::init( std::string const & normal_texture_relative_path, std::string const & swallow_texture_relative_path )
{
    set_schedule_mouse_event( );
    set_schedule_touch_event( );

    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );

    auto normal = normal_texture_relative_path;
    auto swallow = swallow_texture_relative_path;

    assert_log( !app::getAssetPath( normal_texture_relative_path ).empty( ), "ファイルが見つかりません。", return false );
    if ( swallow_texture_relative_path.empty( ) )
    {
        swallow = normal_texture_relative_path;
    }
    else
    {
        assert_log( !app::getAssetPath( swallow_texture_relative_path ).empty( ), "ファイルが見つかりません。", return false );
    }

    _normal_texture = gl::Texture::create( loadImage( app::loadAsset( normal ) ) );
    _swallow_texture = gl::Texture::create( loadImage( app::loadAsset( swallow ) ) );
    _render_texture = _normal_texture;
    _content_size = _normal_texture->getSize( );
    return true;
}
void button::render( )
{
    if ( _render_texture )
    {
        gl::draw( _render_texture, Rectf( vec2( 0 ), _content_size ) );
    }
    else
    {
        gl::drawSolidRect( Rectf( vec2( 0 ), _content_size ) );
        if ( _touch )
        {
            gl::color( ColorA( 1, 1, 1, 0.5F ) );
            gl::drawSolidRect( Rectf( vec2( 0 ), _content_size ) );
        }
    }
}
bool button::mouse_began( cinder::app::MouseEvent event )
{
    _touch = utility::hit_point_plane_2d( shared_from_this( ), event.getPos( ) );
    if ( _touch && on_began ) on_began( );
    _render_texture = _touch ? _swallow_texture : _normal_texture;
    return _touch;
}
void button::mouse_moved( cinder::app::MouseEvent event )
{
    _touch = utility::hit_point_plane_2d( shared_from_this( ), event.getPos( ) );
    _render_texture = _touch ? _swallow_texture : _normal_texture;
}
void button::mouse_ended( cinder::app::MouseEvent event )
{
    if ( _touch && on_ended ) on_ended( );
    _render_texture = _normal_texture;
    _touch = false;
}
bool button::touch_began( cinder::app::TouchEvent::Touch event )
{
    _touch = utility::hit_point_plane_2d( shared_from_this( ), event.getPos( ) );
    if ( _touch && on_began ) on_began( );
    _render_texture = _touch ? _swallow_texture : _normal_texture;
    return _touch;
}
void button::touch_moved( cinder::app::TouchEvent::Touch event )
{
    _touch = utility::hit_point_plane_2d( shared_from_this( ), event.getPos( ) );
    _render_texture = _touch ? _swallow_texture : _normal_texture;
}
void button::touch_ended( cinder::app::TouchEvent::Touch event )
{
    if ( _touch && on_ended ) on_ended( );
    _render_texture = _normal_texture;
    _touch = false;
}
}
