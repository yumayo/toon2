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
struct socket_object
{
    socket_object( asio::io_service& io )
        : socket( io )
    {

    }
    tcp::socket socket;
    boost::array<char, 512> buffer;
};
struct tcp_server::_member
{
    _member( std::string const& port, int num_of_client )
        : io( )
        , acceptor( io, tcp::endpoint( tcp::v4( ), boost::lexical_cast<int>( port ) ) )
        , sockets( )
    {
        for ( int i = 0; i < num_of_client; ++i )
        {
            sockets.emplace_back( io );
        }
    }
    asio::io_service io;
    tcp::acceptor acceptor;
    std::vector<socket_object> sockets;
    bool stop = false;
    std::shared_ptr<std::thread> thread;
};
CREATE_CPP( tcp_server, std::string const& port, int num_of_client )
{
    CREATE( tcp_server, port, num_of_client );
}
tcp_server::~tcp_server( )
{
    _m->io.stop( );
    _m->stop = true;
    _m->thread->join( );
}
bool tcp_server::init( std::string const& port, int num_of_client )
{
    _m.reset( );
    _m = std::make_shared<_member>( port, num_of_client );

    _m->thread = std::make_shared<std::thread>( [ this ]
    {
        while ( !_m->stop )
        {
            for ( auto& obj : _m->sockets )
            {
                _m->acceptor.async_accept( obj.socket, [ this, &obj ] ( asio::error_code const& error )
                {
                    if ( error )
                    {
                        log( "accept failed: %s", error.message( ).c_str( ) );
                    }
                    else
                    {
                        log( "accept correct!" );

                        asio::async_read(
                            obj.socket,
                            asio::buffer( obj.buffer ),
                            asio::transfer_at_least( 4 ),
                            [ this, &obj ] ( const asio::error_code& error, size_t bytes_transferred )
                        {
                            if ( error && error != asio::error::eof )
                            {
                                log( "receive failed: %s", error.message( ).c_str( ) );
                            }
                            else
                            {
                                const char* data = obj.buffer.data( );
                                log( "receive data: %s", data );
                            }
                        } );
                    }
                } );
            }
            _m->io.run( );
        }
    } );
    return true;
}
}