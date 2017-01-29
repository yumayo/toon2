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
        buffer.fill( 0 );
    }
    tcp::socket socket;
    boost::array<char, 512> buffer;
};
struct tcp_server::_member
{
    _member( std::string const& port, int num_of_client )
        : io( )
        , acceptor( )
        , sockets( )
        , thread( )
        , port( port )
    {
        assert_log( 1 <= num_of_client, "クライアントの数は一つ以上でないといけません。" );
        acceptor = std::make_shared<tcp::acceptor>( io, tcp::endpoint( tcp::v4( ), boost::lexical_cast<int>( port ) ) );
        for ( int i = 0; i < num_of_client + 1; ++i )
        {
            sockets.emplace_back( io );
        }
    }
    void async_accept( socket_object& sock_obj );
    bool is_max( );
    asio::io_service io;
    std::shared_ptr<tcp::acceptor> acceptor;
    std::vector<socket_object> sockets;
    std::shared_ptr<std::thread> thread;
    std::string port;
};
void tcp_server::_member::async_accept( socket_object& sock_obj )
{
    log( "socket accept" );
    acceptor->async_accept( sock_obj.socket, [ this, &sock_obj ] ( asio::error_code const& error )
    {
        if ( error )
        {
            log( "accept failed: %s", error.message( ).c_str( ) );
        }
        else
        {
            log( "accept correct!" );

            if ( is_max( ) )
            {
                asio::async_write( sock_obj.socket, asio::buffer( "満員です。" ),
                                   [ ] ( const asio::error_code& error, size_t bytes_transferred )
                {
                    if ( error )
                    {
                        log( "send falid: %s", error.message( ).c_str( ) );
                    }
                    else
                    {
                        log( "accept max clients!" );
                    }
                } );
                sock_obj.socket.close( );
                async_accept( sock_obj );
            }
            else
            {
                asio::async_read(
                    sock_obj.socket,
                    asio::buffer( sock_obj.buffer ),
                    asio::transfer_at_least( 4 ),
                    [ this, &sock_obj ] ( const asio::error_code& error, size_t bytes_transferred )
                {
                    if ( error )
                    {
                        if ( error == asio::error::eof )
                        {
                            log( "client close: %s", error.message( ).c_str( ) );

                            // クライアントがいなくなったソケットは、もう一度接続します。
                            sock_obj.socket.close( );
                            async_accept( sock_obj );
                        }
                        else
                        {
                            log( "receive failed: %s", error.message( ).c_str( ) );
                        }
                    }
                    else
                    {
                        const char* data = sock_obj.buffer.data( );
                        log( "receive data: %s", data );
                        std::fill_n( sock_obj.buffer.begin( ), bytes_transferred, 0 );
                        sock_obj.socket.close( );
                        async_accept( sock_obj );
                    }
                } );
            }
        }
    } );
}
bool tcp_server::_member::is_max( )
{
    int num = 0;
    for ( auto& obj : sockets )
    {
        if ( obj.socket.is_open( ) ) num++;
    }
    return sockets.size( ) == num;
}
CREATE_CPP( tcp_server, std::string const& port, int num_of_client )
{
    CREATE( tcp_server, port, num_of_client );
}
tcp_server::~tcp_server( )
{
    _m->io.stop( );
    _m->thread->join( );
}
bool tcp_server::init( std::string const& port, int num_of_client )
{
    _m.reset( );
    _m = std::make_shared<_member>( port, num_of_client );

    _m->thread = std::make_shared<std::thread>( [ this, num_of_client ]
    {
        for ( int i = 0; i < num_of_client + 1; ++i )
        {
            _m->async_accept( _m->sockets[i] );
        }
        _m->io.run( );
    } );

    return true;
}
}