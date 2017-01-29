#include "tcp_server.h"
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
struct tcp_server::_member
{
    _member( std::string const& port )
        : io_service( )
        , acceptor( io_service, tcp::endpoint( tcp::v4( ), boost::lexical_cast<int>( port ) ) )
        , socket( io_service )
    {

    }
    asio::io_service io_service;
    tcp::acceptor acceptor;
    tcp::socket socket;
    asio::streambuf buffer;
};
CREATE_CPP( tcp_server, std::string const& port )
{
    CREATE( tcp_server, port );
}
bool tcp_server::init( std::string const& port )
{
    _m.reset( );
    _m = std::make_shared<_member>( port );

    return true;
}
void tcp_server::start_accept( )
{
    _m->acceptor.async_accept( _m->socket, [ this ] ( asio::error_code const& error )
    {
        if ( error )
        {
            log( "accept failed: %s", error.message( ).c_str( ) );
        }
        else
        {
            log( "accept correct!" );

            asio::async_read(
                _m->socket,
                _m->buffer,
                asio::transfer_all( ),
                [ this ] ( const asio::error_code& error, size_t bytes_transferred )
            {
                if ( error && error != asio::error::eof )
                {
                    log( "receive failed: %s", error.message( ).c_str( ) );
                }
                else
                {
                    const char* data = asio::buffer_cast<const char*>( _m->buffer.data( ) );
                    log( "receive data: %s", data );
                    _m->buffer.consume( _m->buffer.size( ) );
                }
            } );
        }
    } );
    _m->io_service.run( );
}
}