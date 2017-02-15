#pragma once
#include "tcp_server.h"
#include "tcp_client.h"
namespace network
{
void log_data( char const* data, size_t bytes_transferred );
void lua_setup( sol::state& lua );
}
