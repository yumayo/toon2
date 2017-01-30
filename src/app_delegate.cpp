#include "app_delegate.h"
#include "cinder/gl/gl.h"
#include "cocoslike.h"
#include "network/tcp_server.h"
#include "network/tcp_client.h"
using namespace cinder;
void app_delegate::setup( )
{
    root = node::create( );
    root->set_name( "root" );

    if ( auto p = renderer::button::create( vec2( 100, 100 ) ) )
    {
        p->set_color( { 0, 0, 1, 1 } );
        p->set_position( { 100, 100 } );
        p->set_scale( { 0.5, 0.5 } );
        p->set_rotation( M_PI / 4 );
        p->set_name( "blue rect" );
        root->add_child( p );
    }

    if ( auto p1 = renderer::button::create( vec2( 100, 100 ) ) )
    {
        p1->set_position( { 200, 200 } );
        p1->set_color( { 1, 0, 1, 1 } );
        p1->set_scale( { 2, 2 } );
        p1->set_rotation( M_PI / 3 );
        p1->set_name( "p1 sprite" );
        p1->set_anchor_point( { 0.5, 0.5 } );
        p1->set_pivot( { 0, 0 } );
        auto act1 = action::move_to::create( 2.0F, vec2( 400, 200 ) );
        auto act2 = action::move_to::create( 2.0F, vec2( 200, 200 ) );
        auto seq = action::sequence::create( action::ease<cinder::EaseInOutQuart>::create( act1 ),
                                             action::delay::create( 1.0F ),
                                             action::ease<cinder::EaseInOutQuart>::create( act2 ),
                                             action::remove_self::create( ) );
        //p1->run_action( seq );
        root->add_child( p1 );

        if ( auto p2 = renderer::sprite::create( "hogehoge.png" ) )
        {
            p2->set_position( { 0, 0 } );
            p2->set_scale( { 0.25F, 0.25F } );
            //p2->set_color( { 0, 1, 0, 1 } );
            p2->set_name( "p2 rect" );
            p1->add_child( p2 );

            if ( auto p3 = renderer::button::create( vec2( 100, 100 ) ) )
            {
                p3->set_position( { 100, 0 } );
                p3->set_color( { 0.2F, 1, 0.5F, 1 } );
                p3->set_name( "p3 rect" );
                p2->add_child( p3 );
            }
        }
    }

    if ( auto t = network::tcp_server::create( "25565", 7 ) )
    {
        root->add_child( t );
    }
}
void app_delegate::update( )
{
    auto delta = (float)getElapsedSeconds( ) - (float)_prev_second;
    root->_update( delta );
    _prev_second = getElapsedSeconds( );
}
void app_delegate::draw( )
{
    gl::clear( );
    root->_render( );
}
void app_delegate::mouseDown( cinder::app::MouseEvent event )
{
    root->_mouse_began( event );
}
void app_delegate::mouseDrag( cinder::app::MouseEvent event )
{
    root->_mouse_moved( event );
}
void app_delegate::mouseUp( cinder::app::MouseEvent event )
{
    root->_mouse_ended( event );
}
void app_delegate::touchesBegan( cinder::app::TouchEvent event )
{
    root->_touches_began( event );
}
void app_delegate::touchesMoved( cinder::app::TouchEvent event )
{
    root->_touches_moved( event );
}
void app_delegate::touchesEnded( cinder::app::TouchEvent event )
{
    root->_touches_ended( event );
}