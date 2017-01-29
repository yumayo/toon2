#include "tcp_client.h"
#define ASIO_HAS_STD_ATOMIC
#define ASIO_HAS_BOOST_DATE_TIME
#define BOOST_DATE_TIME_NO_LIB
#include "asio/asio.hpp"
using asio::ip::tcp;
#include "boost/lexical_cast.hpp"
#include "boost/bind.hpp"
#include "../utility.h"
namespace network
{
struct tcp_client::_member
{
    _member( std::string const& ip_address, std::string const& port )
        : io_service( )
        , socket( io_service )
        , ip_address( ip_address )
        , port( port )
    {

    }
    asio::io_service io_service;
    tcp::socket socket;
    std::string ip_address;
    std::string port;
};
CREATE_CPP( tcp_client, std::string const& ip_address, std::string const& port )
{
    CREATE( tcp_client, ip_address, port );
}
bool tcp_client::init( std::string const& ip_address, std::string const& port )
{
    _m.reset( );
    _m = std::make_shared<_member>( ip_address, port );

    return true;
}
void tcp_client::connect( )
{
    _m->socket.async_connect(
        tcp::endpoint( asio::ip::address::from_string( _m->ip_address ), boost::lexical_cast<int>( _m->port ) ),
        [ this ] ( const asio::error_code& error )
    {
        auto s = _m->socket.available( );
        auto o = _m->socket.is_open( );

        if ( error )
        {
            log( "connect failed: %s", error.message( ).c_str( ) );
        }
        else
        {
            log( "connect correct!" );

            asio::async_write(
                _m->socket,
                asio::buffer( "hello" ),
                [ this ] ( const asio::error_code& error, size_t bytes_transferred )
            {
                if ( error )
                {
                    log( "send failed: %s", error.message( ).c_str( ) );
                }
                else
                {
                    log( "send correct!" );
                }
            } );
        }
    } );
    _m->io_service.run( );
}
}