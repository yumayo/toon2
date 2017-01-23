#include "app_delegate.h"
#include "toon2.h"
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
    for ( auto const& c : _children )
    {
        c->clean( );
    }
    _children.clear( );
}
void app_delegate::update( )
{
    auto delta = (float)getElapsedSeconds( ) - (float)_prev_second;
    for ( auto const& c : _children )
    {
        if ( c->get_schedule_update( ) ) c->update( delta );
    }
    _prev_second = getElapsedSeconds( );
}
void app_delegate::draw( )
{
    for ( auto const& c : _children )
    {
        c->draw( );
    }
}
void app_delegate::mouseDown( cinder::app::MouseEvent event )
{
    for ( auto const& c : _children )
    {
        if ( c->get_schedule_mouse_event( ) ) c->mouse_began( event );
    }
}
void app_delegate::mouseUp( cinder::app::MouseEvent event )
{
    for ( auto const& c : _children )
    {
        if ( c->get_schedule_mouse_event( ) ) c->mouse_ended( event );
    }
}
void app_delegate::mouseMove( cinder::app::MouseEvent event )
{
    for ( auto const& c : _children )
    {
        if ( c->get_schedule_mouse_event( ) ) c->mouse_moved( event );
    }
}
void app_delegate::mouseDrag( cinder::app::MouseEvent event )
{
    for ( auto const& c : _children )
    {
        if ( c->get_schedule_mouse_event( ) ) c->mouse_moved( event );
    }
}
void app_delegate::touchesBegan( cinder::app::TouchEvent event )
{
    for ( auto const& c : _children )
    {
        if ( c->get_schedule_touch_event( ) ) c->touches_began( event );
    }
}
void app_delegate::touchesMoved( cinder::app::TouchEvent event )
{
    for ( auto const& c : _children )
    {
        if ( c->get_schedule_touch_event( ) ) c->touches_moved( event );
    }
}
void app_delegate::touchesEnded( cinder::app::TouchEvent event )
{
    for ( auto const& c : _children )
    {
        if ( c->get_schedule_touch_event( ) ) c->touches_ended( event );
    }
}