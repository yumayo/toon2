#include "tcp_handle.h"
namespace network
{
tcp_handle::tcp_handle( asio::io_service & io )
    : socket( io )
    , handle( _ip_address, _port )
{
    buffer.fill( 0 );
}
void tcp_handle::close( )
{
    _ip_address = "";
    _port = "";
    socket.close( );
    buffer.fill( 0 );
}
void tcp_handle::connect( std::string const & ip_address, std::string const & port )
{
    _ip_address = ip_address;
    _port = port;
}
void tcp_handle::buffer_clear( size_t bytes_transferred )
{
    std::fill_n( buffer.begin( ), bytes_transferred, 0 );
}
char const * tcp_handle::c_str( )
{
    return buffer.data( );
}
}
