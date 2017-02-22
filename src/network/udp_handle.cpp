#include "udp_handle.h"
namespace network
{
udp_handle::udp_handle( asio::io_service & io )

void udp_handle::close( )

void udp_handle::connect( std::string const & ip_address, std::string const & port )
{
    _ip_address = ip_address;
    _port = port;
}
void udp_handle::buffer_clear( size_t bytes_transferred )
{
    std::fill_n( buffer.begin( ), bytes_transferred, 0 );
}
char const * udp_handle::c_str( )
{
    return buffer.data( );
}
}
