#include "udp_server.h"
#include "udp_object_member.h"
namespace network
{
CREATE_CPP( udp_server, std::string const& port )
{
    CREATE( udp_server, port );
}
bool udp_server::init( std::string const& port )
{
    _m = std::make_shared<_member>( *this, port );
    return true;
}
void udp_server::write( std::string const & message, std::function<void( )> on_send )
{
    write( message.c_str( ), message.size( ), on_send );
}
void udp_server::write( char const * message, size_t size, std::function<void( )> on_send )
{
    if ( _m->remote_endpoint_buffer.port( ) == 0 ) return;
    _m->write( _m->remote_endpoint_buffer.address( ).to_string( ),
               boost::lexical_cast<std::string>( _m->remote_endpoint_buffer.port( ) ),
               asio::buffer( message, size ),
               on_send );
}
}