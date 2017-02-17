#pragma once
#include "sol.hpp"
namespace utility
{
void lua_setup( sol::state& lua );
std::shared_ptr<sol::state> lua_make( );
}