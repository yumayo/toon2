#include "lua_setup_all.h"
#include "../node.h"
#include "../cinder/cinder.hpp"
#include "../renderer/renderer.hpp"
#include "../action/action.hpp"
#include "../network/network.hpp"
#include "../audio/audio.hpp"
#include "cinder/app/App.h"
#include "string_utility.h"
namespace utility
{
void lua_setup( sol::state& lua )
{
    lua["script_file"] = [ &lua ] ( std::string const& relative_path )
    {
        try
        {
            lua.script_file( cinder::app::getAssetPath( relative_path ).string( ) );
        }
        catch ( sol::error const& error )
        {
            log( error.what( ) );
        }
    };
}
std::shared_ptr<sol::state> lua_make( )
{
    std::shared_ptr<sol::state> lua = std::make_shared<sol::state>( );
    ( *lua )["pi"] = M_PI;

    node::lua_setup( *lua );
    utility::lua_setup( *lua );
    cinder::lua_setup( *lua );
    renderer::lua_setup( *lua );
    action::lua_setup( *lua );
    network::lua_setup( *lua );
    audio::lua_setup( *lua );

    return lua;
}
}