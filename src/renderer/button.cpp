﻿#include "button.h"
#include "cinder/gl/gl.h"
#include "../utility/collision.h"
#include "../utility/assert_log.h"
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
    _touch = hit_point( event.getPos( ) );
    if ( _touch && on_began ) on_began( );
    _render_texture = _touch ? _swallow_texture : _normal_texture;
    return _touch;
}
void button::mouse_moved( cinder::app::MouseEvent event )
{
    _touch = hit_point( event.getPos( ) );
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
    _touch = hit_point( event.getPos( ) );
    if ( _touch && on_began ) on_began( );
    _render_texture = _touch ? _swallow_texture : _normal_texture;
    return _touch;
}
void button::touch_moved( cinder::app::TouchEvent::Touch event )
{
    _touch = hit_point( event.getPos( ) );
    _render_texture = _touch ? _swallow_texture : _normal_texture;
}
void button::touch_ended( cinder::app::TouchEvent::Touch event )
{
    if ( _touch && on_ended ) on_ended( );
    _render_texture = _normal_texture;
    _touch = false;
}
bool button::hit_point( cinder::vec2 point )
{
    auto mat = get_world_matrix( );

    auto obj = mat;
    obj = translate( obj, _position );
    obj = scale( obj, _scale );
    obj = rotate( obj, _rotation );

    auto ma = translate( obj, vec2( -_content_size.x * _anchor_point.x, -_content_size.y * _anchor_point.y ) );
    auto a = vec2( ma[2][0], ma[2][1] );
    auto mb = translate( obj, vec2( _content_size.x * _anchor_point.x, -_content_size.y * _anchor_point.y ) );
    auto b = vec2( mb[2][0], mb[2][1] );
    auto mc = translate( obj, vec2( _content_size.x * _anchor_point.x, _content_size.y * _anchor_point.y ) );
    auto c = vec2( mc[2][0], mc[2][1] );
    auto md = translate( obj, vec2( -_content_size.x * _anchor_point.x, _content_size.y * _anchor_point.y ) );
    auto d = vec2( md[2][0], md[2][1] );

    int hit = 0;
    hit += utility::hit_point_polygon_2d( a, b, c, point );
    hit += utility::hit_point_polygon_2d( a, c, d, point );

    return hit != 0;
}

#define l_class button
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( button
           , l_base( node )
           , "create", sol::overload( [ ] ( vec2 const& size ) { return l_class::create( size ); },
                                      [ ] ( std::string const & normal_texture_relative_path ) { return l_class::create( normal_texture_relative_path ); },
                                      [ ] ( std::string const & normal_texture_relative_path, std::string const & swallow_texture_relative_path ) { return l_class::create( normal_texture_relative_path, swallow_texture_relative_path ); }
           )
           , l_set( on_began )
           , l_set( on_ended )
    );
}
#include "lua_undef.h"
}
