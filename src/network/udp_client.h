#pragma once
#include "udp_object.h"
namespace network
{
class udp_client : public udp_object
{
public:
    CREATE_H( udp_client, std::string const& ip_address, std::string const& port );
    bool init( std::string const& ip_address, std::string const& port );
public:
    void write( std::string const& message, std::function<void( )> on_send = nullptr ) final override;
    void write( char const* message, size_t size, std::function<void( )> on_send = nullptr ) final override;
};
}