#include "app_delegate.h"
#include "cinder/gl/gl.h"
#include "cocoslike.h"
#include "utility/assert_log.h"
#include "boost/range/algorithm/find_if.hpp"
#include "auto_release_pool.h"
using namespace cinder;
using namespace utility;
int const app_delegate::_INVALID_ID = -1;
void app_delegate::setup( )
{
    _root = node::create( );
    _root->set_name( "root" );
    lua_run( );

    //if ( auto s = network::tcp_server::create( "25565", 6 ) )
    //{
    //    s->set_name( "server" );
    //    _root->add_child( s );
    //}

    //char* d = new char[16];
    //d = "hello";
    //if ( auto c = network::tcp_client::create( "127.0.0.1", "25565" ) )
    //{
    //    c->set_name( "client" );
    //    c->write( d );
    //    _root->add_child( c );
    //}
}
void app_delegate::update( )
{
    auto delta = (float)getElapsedSeconds( ) - (float)_prev_second;
    _root->_update( delta );
    _prev_second = getElapsedSeconds( );
    _lua->collect_garbage( );
    auto_release_pool::get_instans( )->collect_garbage( );
}
void app_delegate::draw( )
{
    gl::clear( );
    _root->_render( );
}
void app_delegate::mouseDown( cinder::app::MouseEvent event )
{
    _root->_mouse_began( event );
}
void app_delegate::mouseDrag( cinder::app::MouseEvent event )
{
    _root->_mouse_moved( event );
}
void app_delegate::mouseUp( cinder::app::MouseEvent event )
{
    _root->_mouse_ended( event );
}
void app_delegate::touchesBegan( cinder::app::TouchEvent event )
{
    if ( _touch_id == _INVALID_ID )
    {
        _touch_id = event.getTouches( ).front( ).getId( );
        auto itr = boost::find_if( event.getTouches( ), [ this ] ( cinder::app::TouchEvent::Touch& touch )
        {
            return touch.getId( ) == _touch_id;
        } );
        if ( itr != event.getTouches( ).end( ) ) _root->_touch_began( *itr );
    }

    _root->_touches_began( event );
}
void app_delegate::touchesMoved( cinder::app::TouchEvent event )
{
    auto itr = boost::find_if( event.getTouches( ), [ this ] ( cinder::app::TouchEvent::Touch& touch )
    {
        return touch.getId( ) == _touch_id;
    } );
    if ( itr != event.getTouches( ).end( ) ) _root->_touch_moved( *itr );

    _root->_touches_moved( event );
}
void app_delegate::touchesEnded( cinder::app::TouchEvent event )
{
    auto itr = boost::find_if( event.getTouches( ), [ this ] ( cinder::app::TouchEvent::Touch& touch )
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
void app_delegate::keyDown( cinder::app::KeyEvent event )
{
    if ( event.getCode( ) == cinder::app::KeyEvent::KEY_RETURN )
    {
        _root->remove_all_children( );
        lua_run( );
    }
}
#include "utility/lua_setup_all.h"
void app_delegate::lua_run( )
{
    _lua = utility::lua_make( );
    ( *_lua )["root"] = _root;

    try
    {
        ( *_lua ).script_file( getAssetPath( "main.lua" ).string( ) );
    }
    catch ( sol::error const& error )
    {
        log( error.what( ) );
    }
}
