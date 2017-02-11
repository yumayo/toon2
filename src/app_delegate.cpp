#include "app_delegate.h"
#include "cinder/gl/gl.h"
#include "cocoslike.h"
#include "network/tcp_server.h"
#include "network/tcp_client.h"
#include "cinder/Font.h"
#include "cinder/gl/TextureFont.h"
using namespace cinder;

void app_delegate::setup( )
{
    _root = node::create( );
    _root->set_name( "root" );
    lua_run( );

    //if ( auto t = network::tcp_server::create( "25565", 7 ) )
    //{
    //    root->add_child( t );
    //}
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
void app_delegate::lua_run( )
{
    sol::state lua;
    lua["root"] = _root;
    lua.new_usertype<vec2>( "vec2"
                            , sol::constructors< sol::types<>
                            , sol::types<vec2>
                            , sol::types<float>
                            , sol::types<float, float>
                            >( )
                            );
    lua.new_usertype<ColorA>( "color"
                              , sol::constructors< sol::types<>
                              , sol::types<ColorA>
                              , sol::types<float, float, float, float>
                              >( )
                              );
    lua["pi"] = M_PI;

    renderer::lua_setup( lua );
    action::lua_setup( lua );

    try
    {
        lua.script_file( getAssetPath( "sample.lua" ).string( ) );
    }
    catch ( sol::error const& error )
    {
        log( error.what( ) );
    }
}