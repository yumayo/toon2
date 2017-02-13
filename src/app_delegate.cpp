#include "app_delegate.h"
#include "cinder/gl/gl.h"
#include "cocoslike.h"
#include "network/tcp_server.h"
#include "network/tcp_client.h"
#include "cinder/Font.h"
#include "cinder/gl/TextureFont.h"
#include "utility/assert_log.h"
#include "utility/utf8.h"
using namespace cinder;
using namespace utility;
void app_delegate::setup( )
{
    _root = node::create( );
    _root->set_name( "root" );
    lua_run( );
}
void app_delegate::update( )
{
    auto delta = (float)getElapsedSeconds( ) - (float)_prev_second;
    _root->_update( delta );
    _prev_second = getElapsedSeconds( );
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
    _root->_touches_began( event );
}
void app_delegate::touchesMoved( cinder::app::TouchEvent event )
{
    _root->_touches_moved( event );
}
void app_delegate::touchesEnded( cinder::app::TouchEvent event )
{
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
    lua = utility::lua_make( );
    ( *lua )["root"] = _root;

    try
    {
        ( *lua ).script_file( getAssetPath( "sample.lua" ).string( ) );
    }
    catch ( sol::error const& error )
    {
        log( error.what( ) );
    }
}
