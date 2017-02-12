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

namespace sol
{
template <>
struct lua_size<cinder::vec2> : std::integral_constant<int, 2> {};
template <>
struct lua_size<cinder::ColorA> : std::integral_constant<int, 4> {};
namespace stack
{
template <>
struct checker<cinder::vec2> {
    template <typename Handler>
    static bool check( lua_State* L, int index, Handler&& handler, record& tracking )
    {
        int absolute_index = lua_absindex( L, index );
        bool success =
            stack::check<float>( L, absolute_index - 1, handler ) &&
            stack::check<float>( L, absolute_index - 0, handler );
        tracking.use( 2 );
        return success;
    }
};
template <>
struct getter<cinder::vec2> {
    static cinder::vec2 get( lua_State* L, int index, record& tracking ) {
        int absolute_index = lua_absindex( L, index );
        float x = stack::get<float>( L, absolute_index - 1 );
        float y = stack::get<float>( L, absolute_index - 0 );
        tracking.use( 2 );
        return cinder::vec2 { x, y };
    }
};
template <>
struct pusher<cinder::vec2> {
    static int push( lua_State* L, const cinder::vec2& things ) {
        int amount;
        amount += stack::push( L, things.x );
        amount += stack::push( L, things.y );
        return amount;
    }
};
template <>
struct checker<cinder::ColorA> {
    template <typename Handler>
    static bool check( lua_State* L, int index, Handler&& handler, record& tracking )
    {
        int absolute_index = lua_absindex( L, index );
        bool success =
            stack::check<float>( L, absolute_index - 3, handler ) &&
            stack::check<float>( L, absolute_index - 2, handler ) &&
            stack::check<float>( L, absolute_index - 1, handler ) &&
            stack::check<float>( L, absolute_index - 0, handler );
        tracking.use( 4 );
        return success;
    }
};
template <>
struct getter<cinder::ColorA> {
    static cinder::ColorA get( lua_State* L, int index, record& tracking ) {
        int absolute_index = lua_absindex( L, index );
        float r = stack::get<float>( L, absolute_index - 3 );
        float g = stack::get<float>( L, absolute_index - 2 );
        float b = stack::get<float>( L, absolute_index - 1 );
        float a = stack::get<float>( L, absolute_index - 0 );
        tracking.use( 4 );
        return cinder::ColorA { r, g, b, a };
    }
};
template <>
struct pusher<cinder::ColorA> {
    static int push( lua_State* L, const cinder::ColorA& things ) {
        int amount;
        amount += stack::push( L, things.r );
        amount += stack::push( L, things.g );
        amount += stack::push( L, things.b );
        amount += stack::push( L, things.a );
        return amount;
    }
};
}
}

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
#include "lua_setup_cinder.h"
void app_delegate::lua_run( )
{
    sol::state lua;
    lua["root"] = _root;
    cinder::lua_setup( lua );
    lua["pi"] = M_PI;

    node::lua_setup( lua );
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