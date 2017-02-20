#include "udp_sender.h"
#define ASIO_HAS_STD_ATOMIC
#define ASIO_HAS_BOOST_DATE_TIME
#define BOOST_DATE_TIME_NO_LIB
#include "asio/asio.hpp"
using udp = asio::ip::udp;
#include "../utility/string_utility.h"
using namespace utility;
namespace network
{
struct udp_sender::_member
{
    _member( udp_sender& parent, std::string const & ip_address, std::string const & port );
    ~_member( );
    void write( asio::const_buffers_1 buffer, std::function<void( )> on_send );
    void close( );
    asio::io_service io;
    asio::ip::udp::socket socket;
    std::string port;
    std::string ip_address;
    udp_sender& parent;
};
CREATE_CPP( udp_sender, std::string const & ip_address, std::string const & port )
{
    CREATE( udp_sender, ip_address, port );
}
bool udp_sender::init( std::string const & ip_address, std::string const & port )
{
    _m = std::make_shared<_member>( *this, ip_address, port );
    return true;
}
void udp_sender::write( std::string const & message, std::function<void( )> on_send )
{
    _m->write( asio::buffer( message.c_str( ), message.size( ) ), on_send );
}
void udp_sender::write( char const * message, size_t size, std::function<void( )> on_send )
{
    _m->write( asio::buffer( message, size ), on_send );
}
void udp_sender::close( )
{
    _m->close( );
}
udp_sender::_member::_member( udp_sender& parent, std::string const & ip_address, std::string const & port )
    : parent( parent )
    , socket( io )
    , ip_address( ip_address )
    , port( port )
{
    socket.open( udp::v4( ) );
}
udp_sender::_member::~_member( )
{
    close( );
}
void udp_sender::_member::write( asio::const_buffers_1 buffer, std::function<void( )> on_send )
{
    try
    {
        udp::resolver resolver( io );
        udp::resolver::query query( udp::v4( ), ip_address, port );
        udp::endpoint receiver_endpoint = *resolver.resolve( query );
        auto len = socket.send_to( asio::buffer( buffer ), receiver_endpoint );
        log( "【udp_sender】送信成功！" );
        if ( on_send ) on_send( );
    }
    catch ( asio::error_code& error )
    {
        log( "【udp_sender】送信できませんでした。: %s", error.message( ).c_str( ) );
        if ( parent.on_send_failed ) parent.on_send_failed( );
    }
}
void udp_sender::_member::close( )
{
    if ( parent.on_closed ) parent.on_closed( );
    socket.close( );
}
}
