#include "app_delegate.h"
#include "cinder/gl/gl.h"
#include "cocoslike.h"
#include "utility/assert_log.h"
#include "user/title.h"
#include "user/game.h"
using namespace cinder;
using namespace utility;
int const app_delegate::_INVALID_ID = -1;
void app_delegate::setup( )
{
    log( "stand by ready!" );

    _root = node::create( );
    _root->set_name( "root" );
    _root->add_child( user::game::create( ) );
}
void app_delegate::update( )
{
    auto delta = (float)getElapsedSeconds( ) - (float)_prev_second;
    _root->_update( delta );
    _prev_second = getElapsedSeconds( );
}
void app_delegate::draw( )
{
    gl::clear( ColorA( 0.1, 0.1, 0.1, 1.0 ) );
    _root->_render( );
}
void app_delegate::mouseDown( cinder::app::MouseEvent event )
{
    if ( isMultiTouchEnabled( ) ) return;
    _root->_mouse_began( event );
}
void app_delegate::mouseDrag( cinder::app::MouseEvent event )
{
    if ( isMultiTouchEnabled( ) ) return;
    _root->_mouse_moved( event );
}
void app_delegate::mouseUp( cinder::app::MouseEvent event )
{
    if ( isMultiTouchEnabled( ) ) return;
    _root->_mouse_ended( event );
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
        if ( itr != event.getTouches( ).end( ) ) _root->_touch_began( *itr );
    }

    _root->_touches_began( event );
}
void app_delegate::touchesMoved( cinder::app::TouchEvent event )
{
    auto itr = std::find_if( std::begin( event.getTouches( ) ), std::end( event.getTouches( ) ), [ this ] ( cinder::app::TouchEvent::Touch& touch )
    {
        return touch.getId( ) == _touch_id;
    } );
    if ( itr != event.getTouches( ).end( ) ) _root->_touch_moved( *itr );

    _root->_touches_moved( event );
}
void app_delegate::touchesEnded( cinder::app::TouchEvent event )
{
    auto itr = std::find_if( std::begin( event.getTouches( ) ), std::end( event.getTouches( ) ), [ this ] ( cinder::app::TouchEvent::Touch& touch )
    {
        return touch.getId( ) == _touch_id;
    } );
    if ( itr != event.getTouches( ).end( ) )
    {
        _root->_touch_ended( *itr );
        _touch_id = _INVALID_ID;
    }

    _root->_touches_ended( event );
}