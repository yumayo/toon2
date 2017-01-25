#include "app_delegate.h"
#include "cinder/gl/gl.h"
#include "toon2.h"
using namespace cinder;
app_delegate::app_delegate( )
{
    set_name( "app_delegate" );
}
app_delegate::~app_delegate( )
{
    /* nothing */
}
void app_delegate::setup( )
{
    auto p = toon2::create( );
    p->set_name( "test" );
    add_child( p );
}
void app_delegate::cleanup( )
{
    node::clean( );
}
void app_delegate::update( )
{
    auto delta = (float)getElapsedSeconds( ) - (float)_prev_second;
    node::update( delta );
    _prev_second = getElapsedSeconds( );
}
void app_delegate::draw( )
{
    gl::clear( );
    node::renderer( );
}
void app_delegate::mouseDown( cinder::app::MouseEvent event )
{
    node::mouse_began( event );
}
void app_delegate::mouseUp( cinder::app::MouseEvent event )
{
    node::mouse_ended( event );
}
void app_delegate::mouseMove( cinder::app::MouseEvent event )
{
    node::mouse_moved( event );
}
void app_delegate::mouseDrag( cinder::app::MouseEvent event )
{
    node::mouse_moved( event );
}
void app_delegate::touchesBegan( cinder::app::TouchEvent event )
{
    node::touches_began( event );
}
void app_delegate::touchesMoved( cinder::app::TouchEvent event )
{
    node::touches_moved( event );
}
void app_delegate::touchesEnded( cinder::app::TouchEvent event )
{
    node::touches_ended( event );
}