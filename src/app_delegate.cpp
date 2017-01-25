#include "app_delegate.h"
#include "cinder/gl/gl.h"
#include "renderer/rect.h"
#include "renderer/button.h"
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
    if ( auto p = renderer::button::create( vec2( 100, 100 ) ) )
    {
        p->set_color( { 0, 0, 1, 1 } );
        p->set_position( { 100, 100 } );
        p->set_scale( { 0.5, 0.5 } );
        p->set_rotation( M_PI / 4 );
        p->set_name( "blue rect" );
        add_child( p );
    }
    
    if ( auto p = renderer::button::create( vec2( 100, 100 ) ) )
    {
        p->set_position( { 300, 300 } );
        p->set_color( { 1, 0, 0, 1 } );
        p->set_scale( { 2, 2 } );
        p->set_rotation( M_PI / 3 );
        p->set_name( "red rect" );
        p->set_anchor_point( { 0.5, 0.5 } );
        p->set_pivot( { 0, 0 } );
        add_child( p );

        if ( auto pp = renderer::button::create( vec2( 50, 50 ) ) )
        {
            pp->set_position( { 0, 0 } );
            pp->set_color( { 0, 1, 0, 1 } );
            pp->set_name( "green rect" );
            p->add_child( pp );
        }
    }
}
void app_delegate::update( )
{
    auto delta = (float)getElapsedSeconds( ) - (float)_prev_second;
    node::_update( delta );
    _prev_second = getElapsedSeconds( );
}
void app_delegate::draw( )
{
    gl::clear( );
    node::_render( );
}
void app_delegate::mouseDown( cinder::app::MouseEvent event )
{
    node::_mouse_began( event );
}
void app_delegate::mouseDrag( cinder::app::MouseEvent event )
{
    node::_mouse_moved( event );
}
void app_delegate::mouseUp( cinder::app::MouseEvent event )
{
    node::_mouse_ended( event );
}
void app_delegate::touchesBegan( cinder::app::TouchEvent event )
{
    node::_touches_began( event );
}
void app_delegate::touchesMoved( cinder::app::TouchEvent event )
{
    node::_touches_moved( event );
}
void app_delegate::touchesEnded( cinder::app::TouchEvent event )
{
    node::_touches_ended( event );
}