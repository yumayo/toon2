#include "button.h"
#include "cinder/gl/gl.h"
#include "../utility.h"
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
    _touch = hit_point( event.getPos( ) );
    return _touch;
}
void button::mouse_moved( cinder::app::MouseEvent event )
{
    _touch = hit_point( event.getPos( ) );
}
void button::mouse_ended( cinder::app::MouseEvent event )
{
    _touch = false;
}
bool button::touches_began( cinder::app::TouchEvent event )
{
    if ( hit_point( event.getTouches( )[0].getPos( ) ) )
    {
        _touch = true;
        _swallow = true;
        _id = event.getTouches( )[0].getId( );

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

    _touch = hit_point( event.getTouches( )[0].getPos( ) );
}
void button::touches_ended( cinder::app::TouchEvent event )
{
    if ( !_swallow ) return;

    for ( auto& touch : event.getTouches( ) )
    {
        if ( touch.getId( ) == _id )
        {
            _touch = false;
        }
    }
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
    hit += hit_point_polygon_2d( a, b, c, point );
    hit += hit_point_polygon_2d( a, c, d, point );

    return hit != 0;
}
}
