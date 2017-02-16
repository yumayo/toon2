
#include "socket_object.h"
namespace network
{
char const * socket_object::c_str( )
{
    return buffer.data( );
}
void socket_object::handshake( )
{
    handle.handshake( c_str( ) );
}
void socket_object::buffer_clear( size_t bytes_transferred )
{
    std::fill_n( buffer.begin( ), bytes_transferred, 0 );
}
void socket_object::close( )
{
    handle.clear( );
    socket.close( );
    buffer.fill( 0 );
}
socket_object::socket_object( asio::io_service & io )
    : socket( io )
{
    buffer.fill( 0 );
}
}
