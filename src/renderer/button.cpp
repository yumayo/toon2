﻿#include "button.h"
#include "cinder/gl/gl.h"
using namespace cinder;
namespace renderer
{
CREATE_CPP( button, cinder::vec2 const & size )
{
    CREATE( button, size );
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
void button::render( )
{
    gl::drawSolidRect( Rectf( vec2( 0 ), _content_size ) );

    if ( _touch )
    {
        gl::color( ColorA( 1, 1, 1, 0.5F ) );
        gl::drawSolidRect( Rectf( vec2( 0 ), _content_size ) );
    }
}
bool button::mouse_began( cinder::app::MouseEvent event )
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
    hit += hit_point_polygon_2d( a, b, c, event.getPos( ) );
    hit += hit_point_polygon_2d( a, c, d, event.getPos( ) );

    if ( hit != 0 )
    {
        _touch = true;
        _swallow = true;
        return true;
    }
    else
    {
        return false;
    }
}
void button::mouse_moved( cinder::app::MouseEvent event )
{
    if ( !_swallow ) return;

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
    hit += hit_point_polygon_2d( a, b, c, event.getPos( ) );
    hit += hit_point_polygon_2d( a, c, d, event.getPos( ) );

    _touch = hit != 0;
}
void button::mouse_ended( cinder::app::MouseEvent event )
{
    if ( !_swallow ) return;

    _touch = false;
}
bool button::touches_began( cinder::app::TouchEvent event )
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
    hit += hit_point_polygon_2d( a, b, c, event.getTouches()[0].getPos( ) );
    hit += hit_point_polygon_2d( a, c, d, event.getTouches()[0].getPos( ) );
    
    if ( hit != 0 )
    {
        _touch = true;
        _swallow = true;
        return true;
    }
    else
    {
        return false;
    }
}
    void button::touches_moved( cinder::app::TouchEvent event )
{
    if ( !_swallow ) return;
    
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
    hit += hit_point_polygon_2d( a, b, c, event.getTouches()[0].getPos( ) );
    hit += hit_point_polygon_2d( a, c, d, event.getTouches()[0].getPos( ) );
    
    _touch = hit != 0;
}
    void button::touches_ended( cinder::app::TouchEvent event )
{
    if ( !_swallow ) return;
    
    _touch = false;
}
}
