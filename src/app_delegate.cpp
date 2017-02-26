#include "app_delegate.h"
#include "cinder/gl/gl.h"
#include "cocoslike.h"
#include "utility/assert_log.h"
#include "scene_manager.h"
#include "user/title.h"
using namespace cinder;
using namespace utility;
int const app_delegate::_INVALID_ID = -1;
void app_delegate::setup( )
{
    log( "stand by ready!" );

    scene_manager::get_instans( )->push( user::title::create( ) );
    scene_manager::get_instans( )->update( );
}
void app_delegate::update( )
{
    auto delta = (float)getElapsedSeconds( ) - (float)_prev_second;
    scene_manager::get_instans( )->top( )->_update( delta );
    _prev_second = getElapsedSeconds( );
    scene_manager::get_instans( )->update( );
}
void app_delegate::draw( )
{
    gl::clear( ColorA( 0.1, 0.1, 0.1, 1.0 ) );
    scene_manager::get_instans( )->top( )->_render( );
}
void app_delegate::mouseDown( cinder::app::MouseEvent event )
{
    if ( isMultiTouchEnabled( ) ) return;
    scene_manager::get_instans( )->top( )->_mouse_began( event );
}
void app_delegate::mouseDrag( cinder::app::MouseEvent event )
{
    if ( isMultiTouchEnabled( ) ) return;
    scene_manager::get_instans( )->top( )->_mouse_moved( event );
}
void app_delegate::mouseUp( cinder::app::MouseEvent event )
{
    if ( isMultiTouchEnabled( ) ) return;
    scene_manager::get_instans( )->top( )->_mouse_ended( event );
}
void app_delegate::touchesBegan( cinder::app::TouchEvent event )
{
    if ( _touch_id == _INVALID_ID )
    {
        _touch_id = event.getTouches( ).front( ).getId( );
        auto itr = std::find_if( std::begin( event.getTouches( ) ), std::end( event.getTouches( ) ), [ this ] ( cinder::app::TouchEvent::Touch& touch )
        {
            return touch.getId( ) == _touch_id;
        } );
        if ( itr != event.getTouches( ).end( ) ) scene_manager::get_instans( )->top( )->_touch_began( *itr );
    }

    scene_manager::get_instans( )->top( )->_touches_began( event );
}
void app_delegate::touchesMoved( cinder::app::TouchEvent event )
{
    auto itr = std::find_if( std::begin( event.getTouches( ) ), std::end( event.getTouches( ) ), [ this ] ( cinder::app::TouchEvent::Touch& touch )
    {
        return touch.getId( ) == _touch_id;
    } );
    if ( itr != event.getTouches( ).end( ) ) scene_manager::get_instans( )->top( )->_touch_moved( *itr );

    scene_manager::get_instans( )->top( )->_touches_moved( event );
}
void app_delegate::touchesEnded( cinder::app::TouchEvent event )
{
    auto itr = std::find_if( std::begin( event.getTouches( ) ), std::end( event.getTouches( ) ), [ this ] ( cinder::app::TouchEvent::Touch& touch )
    {
        return touch.getId( ) == _touch_id;
    } );
    if ( itr != event.getTouches( ).end( ) )
    {
        scene_manager::get_instans( )->top( )->_touch_ended( *itr );
        _touch_id = _INVALID_ID;
    }

    scene_manager::get_instans( )->top( )->_touches_ended( event );
}