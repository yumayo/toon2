#include "udp_server.h"
#include "udp_server_member.h"
namespace network
{
CREATE_CPP( udp_server, std::string const& port )
{
    CREATE( udp_server, port );
}
bool udp_server::init( std::string const& port )
{
    _m = std::make_shared<_member>( *this, port );
    set_schedule_update( );
    return true;
}
void udp_server::update( float delta )
{
    _m->update( );
}
void udp_server::write( std::string const & message, std::function<void( )> on_send )
{
    write( message.c_str( ), message.size( ), on_send );
}
void udp_server::write( char const * message, size_t size, std::function<void( )> on_send )
{
    _m->write( asio::buffer( message, size ),
               on_send );
}
void udp_server::close( )
{
    _m->close( );
}
}