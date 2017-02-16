#pragma once
#include "sol.hpp"
namespace network
{
void log_data( char const* data, size_t bytes_transferred );
void lua_setup( sol::state& lua );
}
