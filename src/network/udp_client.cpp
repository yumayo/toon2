#include "udp_client.h"
#include "udp_object_member.h"
namespace network
{
CREATE_CPP( udp_client, std::string const & ip_address, std::string const & port )
{
    CREATE( udp_client, ip_address, port );
}
bool udp_client::init( std::string const & ip_address, std::string const & port )
{
    _m = std::make_shared<_member>( *this, ip_address, port );
    return true;
}
void udp_client::write( std::string const & message, std::function<void( )> on_send )
{
    write( message.c_str( ), message.size( ), on_send );
}
void udp_client::write( char const * message, size_t size, std::function<void( )> on_send )
{
    _m->write( _m->ip_address, _m->port, asio::buffer( message, size ), on_send );
}
}