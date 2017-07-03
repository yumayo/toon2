#include "button_scale.h"
#include "cinder/gl/gl.h"
#include "utility/collision.h"
#include "utility/assert_log.h"
#include "action.hpp"
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
    _touch = utility::hit_point_plane_2d_default_size( event.getPos( ), shared_from_this( ) );
    if ( _touch && on_began ) on_began( );
    if ( _touch )
    {
        remove_action_by_name( "button_scale" );
        set_scale( vec2( 1.0F ) );
        auto act = action::ease<EaseOutBounce>::create( action::scale_to::create( 0.1, vec2( 1.2 ) ) );
        act->set_name( "button_scale" );
        run_action( act );
    }
    return _touch;
}
void button_scale::mouse_moved( cinder::app::MouseEvent event )
{
    _touch = utility::hit_point_plane_2d_default_size( event.getPos( ), shared_from_this( ) );
    // in
    if ( ( true == _touch ) && ( false == _prev_touch ) )
    {
        remove_action_by_name( "button_scale" );
        set_scale( vec2( 1.0F ) );
        auto act = action::ease<EaseOutBounce>::create( action::scale_to::create( 0.1, vec2( 1.2 ) ) );
        act->set_name( "button_scale" );
        run_action( act );
    }
    // out
    if ( ( false == _touch ) && ( true == _prev_touch ) )
    {
        remove_action_by_name( "button_scale" );
        set_scale( vec2( 1.2F ) );
        auto act = action::ease<EaseOutBounce>::create( action::scale_to::create( 0.1, vec2( 1.0 ) ) );
        act->set_name( "button_scale" );
        run_action( act );
    }
}
void button_scale::mouse_ended( cinder::app::MouseEvent event )
{
    if ( _touch && on_ended ) on_ended( );
    if ( _touch )
    {
        remove_action_by_name( "button_scale" );
        set_scale( vec2( 1.2F ) );
        auto act = action::ease<EaseOutBounce>::create( action::scale_to::create( 0.1, vec2( 1.0 ) ) );
        act->set_name( "button_scale" );
        run_action( act );
    }
    _touch = false;
}
bool button_scale::touch_began( cinder::app::TouchEvent::Touch event )
{
    _touch = utility::hit_point_plane_2d_default_size( event.getPos( ), shared_from_this( ) );
    if ( _touch && on_began ) on_began( );
    if ( _touch )
    {
        remove_action_by_name( "button_scale" );
        set_scale( vec2( 1.0F ) );
        auto act = action::ease<EaseOutBounce>::create( action::scale_to::create( 0.1, vec2( 1.2 ) ) );
        act->set_name( "button_scale" );
        run_action( act );
    }
    return _touch;
}
void button_scale::touch_moved( cinder::app::TouchEvent::Touch event )
{
    _touch = utility::hit_point_plane_2d_default_size( event.getPos( ), shared_from_this( ) );
    // in
    if ( ( true == _touch ) && ( false == _prev_touch ) )
    {
        remove_action_by_name( "button_scale" );
        set_scale( vec2( 1.0F ) );
        auto act = action::ease<EaseOutBounce>::create( action::scale_to::create( 0.1, vec2( 1.2 ) ) );
        act->set_name( "button_scale" );
        run_action( act );
    }
    // out
    if ( ( false == _touch ) && ( true == _prev_touch ) )
    {
        remove_action_by_name( "button_scale" );
        set_scale( vec2( 1.2F ) );
        auto act = action::ease<EaseOutBounce>::create( action::scale_to::create( 0.1, vec2( 1.0 ) ) );
        act->set_name( "button_scale" );
        run_action( act );
    }
}
void button_scale::touch_ended( cinder::app::TouchEvent::Touch event )
{
    if ( _touch && on_ended ) on_ended( );
    if ( _touch )
    {
        remove_action_by_name( "button_scale" );
        set_scale( vec2( 1.2F ) );
        auto act = action::ease<EaseOutBounce>::create( action::scale_to::create( 0.1, vec2( 1.0 ) ) );
        act->set_name( "button_scale" );
        run_action( act );
    }
    _touch = false;
}
}
