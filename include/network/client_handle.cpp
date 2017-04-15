#include "client_handle.h"
namespace network
{
client_handle::client_handle( std::string const & ip_address, std::string const & port )
    : ip_address( ip_address )
    , port( port )
{

}
bool client_handle::operator==( client_handle const & other ) const
{
    return this->ip_address == other.ip_address && this->port == other.ip_address;
}
std::string client_handle::get_ip_address( ) const
{
    return ip_address;
}
std::string client_handle::get_port( ) const
{
    return port;
}
}
