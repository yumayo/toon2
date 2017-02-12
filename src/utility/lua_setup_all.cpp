#include "lua_setup_all.h"
#include "../node.h"
#include "../cinder/cinder.hpp"
#include "../renderer/renderer.hpp"
#include "../action/action.hpp"
namespace utility
{
std::shared_ptr<sol::state> lua_make( )
{
    std::shared_ptr<sol::state> lua = std::make_shared<sol::state>( );
    ( *lua )["pi"] = M_PI;

    node::lua_setup( ( *lua ) );
    cinder::lua_setup( ( *lua ) );
    renderer::lua_setup( ( *lua ) );
    action::lua_setup( ( *lua ) );

    return lua;
}
}