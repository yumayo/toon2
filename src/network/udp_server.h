#pragma once
#include "udp_object.h"
namespace network
{
class udp_server : public udp_object
{
public:
    CREATE_H( udp_server, std::string const& port );
    bool init( std::string const& port );
public:
    void write( std::string const& message, std::function<void( )> on_send = nullptr ) final override;
    void write( char const* message, size_t size, std::function<void( )> on_send = nullptr ) final override;
};
}