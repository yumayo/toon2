#include "button_scale.h"
#include "cinder/gl/gl.h"
#include "../utility/collision.h"
#include "../utility/assert_log.h"
#include "../action/action.hpp"
using namespace cinder;
namespace user
{
CREATE_CPP( button_scale )
{
    CREATE( button_scale );
}
bool button_scale::init( )
{
    set_schedule_mouse_event( );
    set_schedule_touch_event( );

    set_schedule_update( );

    return true;
}
void button_scale::update( float delta )
{
    _prev_touch = _touch;
}
bool button_scale::mouse_began( cinder::app::MouseEvent event )
{
    _touch = utility::hit_point_plane_2d_default_size( shared_from_this( ), event.getPos( ) );
    if ( _touch && on_began ) on_began( );
    if ( _touch )
    {
        remove_all_actions( );
        set_scale( vec2( 1.0F ) );
        run_action( action::ease<EaseOutBounce>::create( action::scale_to::create( 0.1, vec2( 1.2 ) ) ) );
    }
    return _touch;
}
void button_scale::mouse_moved( cinder::app::MouseEvent event )
{
    _touch = utility::hit_point_plane_2d_default_size( shared_from_this( ), event.getPos( ) );
    // in
    if ( ( true == _touch ) && ( false == _prev_touch ) )
    {
        remove_all_actions( );
        set_scale( vec2( 1.0F ) );
        run_action( action::ease<EaseOutBounce>::create( action::scale_to::create( 0.1, vec2( 1.2 ) ) ) );
    }
    // out
    if ( ( false == _touch ) && ( true == _prev_touch ) )
    {
        remove_all_actions( );
        set_scale( vec2( 1.2F ) );
        run_action( action::ease<EaseOutBounce>::create( action::scale_to::create( 0.1, vec2( 1.0 ) ) ) );
    }
}
void button_scale::mouse_ended( cinder::app::MouseEvent event )
{
    if ( _touch && on_ended ) on_ended( );
    if ( _touch )
    {
        remove_all_actions( );
        set_scale( vec2( 1.2F ) );
        run_action( action::ease<EaseOutBounce>::create( action::scale_to::create( 0.1, vec2( 1.0 ) ) ) );
    }
    _touch = false;
}
bool button_scale::touch_began( cinder::app::TouchEvent::Touch event )
{
    _touch = utility::hit_point_plane_2d_default_size( shared_from_this( ), event.getPos( ) );
    if ( _touch && on_began ) on_began( );
    if ( _touch )
    {
        remove_all_actions( );
        set_scale( vec2( 1.0F ) );
        run_action( action::ease<EaseOutBounce>::create( action::scale_to::create( 0.1, vec2( 1.2 ) ) ) );
    }
    return _touch;
}
void button_scale::touch_moved( cinder::app::TouchEvent::Touch event )
{
    _touch = utility::hit_point_plane_2d_default_size( shared_from_this( ), event.getPos( ) );
    // in
    if ( ( true == _touch ) && ( false == _prev_touch ) )
    {
        remove_all_actions( );
        set_scale( vec2( 1.0F ) );
        run_action( action::ease<EaseOutBounce>::create( action::scale_to::create( 0.1, vec2( 1.2 ) ) ) );
    }
    // out
    if ( ( false == _touch ) && ( true == _prev_touch ) )
    {
        remove_all_actions( );
        set_scale( vec2( 1.2F ) );
        run_action( action::ease<EaseOutBounce>::create( action::scale_to::create( 0.1, vec2( 1.0 ) ) ) );
    }
}
void button_scale::touch_ended( cinder::app::TouchEvent::Touch event )
{
    if ( _touch && on_ended ) on_ended( );
    if ( _touch )
    {
        remove_all_actions( );
        set_scale( vec2( 1.2F ) );
        run_action( action::ease<EaseOutBounce>::create( action::scale_to::create( 0.1, vec2( 1.0 ) ) ) );
    }
    _touch = false;
}
}
