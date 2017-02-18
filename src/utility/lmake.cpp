#include "lmake.h"
#include "../node.h"
#include "../cinder/cinder.hpp"
#include "../renderer/renderer.hpp"
#include "../action/action.hpp"
#include "../network/network.hpp"
#include "../audio/audio.hpp"
#include "cinder/app/App.h"
#include "string_utility.h"
#include "assert_log.h"
namespace utility
{
sol::state* lmake( std::string const & relative_path, std::shared_ptr<node> const & root )
{
    sol::state* p = new sol::state;
    auto& lua = *p;
    lua["pi"] = M_PI;
    lua["root"] = root;

    node::lua_setup( lua );
    cinder::lua_setup( lua );
    renderer::lua_setup( lua );
    action::lua_setup( lua );
    network::lua_setup( lua );
    audio::lua_setup( lua );

    assert_log( !cinder::app::getAssetPath( relative_path ).empty( ), "ファイルが見つかりません。", return nullptr );
    lua.script_file( cinder::app::getAssetPath( relative_path ).string( ) );
    return p;
}
}