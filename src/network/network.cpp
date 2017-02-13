#pragma once
#include "network.hpp"
namespace network
{
void lua_setup( sol::state& lua )
{
    tcp_server::lua_setup( lua );
    tcp_client::lua_setup( lua );
}
}
