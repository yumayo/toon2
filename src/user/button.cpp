#include "button.h"
#include "cinder/gl/gl.h"
#include <treelike/utility/collision.h>
#include <treelike/utility/assert_log.h>
#include <treelike/action.hpp>
using namespace cinder;
using namespace treelike;
namespace user
{
bool base_button::init( )
{
    set_schedule_mouse_event( );
    set_schedule_touch_event( );

    set_schedule_update( );

    return true;
}
void base_button::update( float delta )
{
    _prev_touch = _touch;
}
bool base_button::mouse_began( cinder::app::MouseEvent event )
{
    return began( event.getPos( ) );
}
void base_button::mouse_moved( cinder::app::MouseEvent event )
{
    moved( event.getPos( ) );
}
void base_button::mouse_ended( cinder::app::MouseEvent event )
{
    ended( event.getPos( ) );
}
bool base_button::touch_began( cinder::app::TouchEvent::Touch event )
{
    return began( event.getPos( ) );
}
void base_button::touch_moved( cinder::app::TouchEvent::Touch event )
{
    moved( event.getPos( ) );
}
void base_button::touch_ended( cinder::app::TouchEvent::Touch event )
{
    ended( event.getPos( ) );
}
void base_button::touches_began( cinder::app::TouchEvent event )
{
    for ( auto& touch : event.getTouches( ) ) began( touch.getPos( ) );
}
void base_button::touches_moved( cinder::app::TouchEvent event )
{
    for ( auto& touch : event.getTouches( ) ) moved( touch.getPos( ) );
}
void base_button::touches_ended( cinder::app::TouchEvent event )
{
    for ( auto& touch : event.getTouches( ) ) ended( touch.getPos( ) );
}
bool base_button::began( cinder::vec2 touch_position )
{
    _touch = hit_touch( touch_position );
    if ( _touch && on_began ) on_began( );
    if ( _touch )
    {
        in_action( );
    }
    return _touch;
}
void base_button::moved( cinder::vec2 touch_position )
{
    _touch = hit_touch( touch_position );
    if ( ( true == _touch ) && ( false == _prev_touch ) )
    {
        in_action( );
    }
    if ( ( false == _touch ) && ( true == _prev_touch ) )
    {
        out_action( );
    }
}
void base_button::ended( cinder::vec2 touch_position )
{
    if ( _touch && on_ended ) on_ended( );
    if ( _touch )
    {
        out_action( );
    }
    _touch = false;
}
bool base_rect_button::hit_touch( cinder::vec2 touch_position )
{
    return utility::hit_point_rect_non_scale( touch_position, shared_from_this( ) );
}
bool base_ciecle_button::hit_touch( cinder::vec2 touch_position )
{
    return utility::hit_point_ciecle_non_scale( touch_position, shared_from_this( ) );
}
bool base_anytype_button_opacity_action::init( )
{
    base_button::init( );
    set_opacity( 0.5F );
    return true;
}
void base_anytype_button_opacity_action::in_action( )
{
    remove_action_by_name( "button_action" );
    set_opacity( 0.5F );
    auto act = action::ease<EaseOutCirc>::create( action::opacity_to::create( 0.3F, 1.0F ) );
    act->set_name( "button_action" );
    run_action( act );
}
void base_anytype_button_opacity_action::out_action( )
{
    remove_action_by_name( "button_action" );
    set_opacity( 1.0F );
    auto act = action::ease<EaseOutCirc>::create( action::opacity_to::create( 0.3F, 0.5F ) );
    act->set_name( "button_action" );
    run_action( act );
}
bool base_anytype_button_scale_action::init( )
{
    base_button::init( );
    set_scale( vec2( 1.0F ) );
    return true;
}
void base_anytype_button_scale_action::in_action( )
{
    remove_action_by_name( "button_action" );
    set_scale( vec2( 1.0F ) );
    auto act = action::ease<EaseOutBounce>::create( action::scale_to::create( 0.3F, vec2( 1.2F ) ) );
    act->set_name( "button_action" );
    run_action( act );
}
void base_anytype_button_scale_action::out_action( )
{
    remove_action_by_name( "button_action" );
    set_scale( vec2( 1.2F ) );
    auto act = action::ease<EaseOutBounce>::create( action::scale_to::create( 0.3F, vec2( 1.0F ) ) );
    act->set_name( "button_action" );
    run_action( act );
}
CREATE_CPP( rect_button_opacity_action )
{
    CREATE( rect_button_opacity_action );
}
CREATE_CPP( circle_button_opacity_action )
{
    CREATE( circle_button_opacity_action );
}
CREATE_CPP( rect_button_scale_action )
{
    CREATE( rect_button_scale_action );
}
CREATE_CPP( circle_button_scale_action )
{
    CREATE( circle_button_scale_action );
}
}
