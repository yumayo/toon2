#include "tcp_server.h"
#define ASIO_HAS_STD_ATOMIC
#define ASIO_HAS_BOOST_DATE_TIME
#define BOOST_DATE_TIME_NO_LIB
#include "asio/asio.hpp"
using asio::ip::tcp;
#include "boost/lexical_cast.hpp"
#include "boost/bind.hpp"
#include "../utility/assert_log.h"
#include <set>
using namespace utility;
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
tcp_server::connect_data::connect_data( std::string const & ip_address, int port )
    : ip_address( ip_address )
    , port( port )
{ }
bool tcp_server::connect_data::operator==( connect_data const& other ) const
{
    return this->port == other.port;
}
bool tcp_server::connect_data::operator<( connect_data const& other ) const
{
    return this->port < other.port;
}
struct tcp_server::_member
{
    _member( std::string const& port, int num_of_client )
        : io( )
        , acceptor( )
        , sockets( )
        , thread( )
        , port( port )
    {
        assert_log( 1 <= num_of_client, "クライアントの数は一つ以上でないといけません。", return );
        acceptor = std::make_shared<tcp::acceptor>( io, tcp::endpoint( tcp::v4( ), boost::lexical_cast<int>( port ) ) );
        // 一つはクライアントの接続を切る役割を持たせています。
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
    std::set<connect_data> ip_data;
};
void tcp_server::_member::async_accept( socket_object& sock_obj )
{
    log( "ソケットの準備" );
    acceptor->async_accept( sock_obj.socket, [ this, &sock_obj ] ( asio::error_code const& error )
    {
        if ( error )
        {
            log( "接続は無効になりました。: %s", error.message( ).c_str( ) );
        }
        else
        {
            log( "接続を受け付けました。" );
            if ( is_max( ) )
            {
                asio::async_write( sock_obj.socket, asio::buffer( "満員です。" ),
                                   [ ] ( const asio::error_code& error, size_t bytes_transferred )
                {
                    if ( error )
                    {
                        log( "データを送れませんでした。: %s", error.message( ).c_str( ) );
                    }
                    else
                    {
                        log( "接続数が多いため、アクセスは拒否されました。" );
                    }
                } );
                sock_obj.socket.close( );
                async_accept( sock_obj );
            }
            else
            {
                log( "接続成功！: %s, %d", sock_obj.socket.remote_endpoint( ).address( ).to_string( ).c_str( ), sock_obj.socket.remote_endpoint( ).port( ) );
                ip_data.insert( connect_data( sock_obj.socket.remote_endpoint( ).address( ).to_string( ),
                                              sock_obj.socket.remote_endpoint( ).port( ) ) );

                asio::async_read(
                    sock_obj.socket,
                    asio::buffer( sock_obj.buffer ),
                    asio::transfer_at_least( 1 ),
                    [ this, &sock_obj ] ( const asio::error_code& error, size_t bytes_transferred )
                {
                    if ( error )
                    {
                        if ( error == asio::error::eof )
                        {
                            log( "クライアントが接続を切りました。: %s", error.message( ).c_str( ) );
                            ip_data.erase( connect_data( sock_obj.socket.remote_endpoint( ).address( ).to_string( ),
                                                         sock_obj.socket.remote_endpoint( ).port( ) ) );

                            // クライアントがいなくなったソケットは、もう一度接続します。
                            sock_obj.socket.close( );
                            async_accept( sock_obj );
                        }
                        else
                        {
                            log( "無効なアクセスです。: %s", error.message( ).c_str( ) );
                        }
                    }
                    else
                    {
                        const char* data = sock_obj.buffer.data( );
                        log( "データ: %s", data );
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
std::vector<std::pair<std::string, int>> tcp_server::get_ip_datas( )
{
    std::vector<std::pair<std::string, int>> ret;
    for ( auto& obj : _m->ip_data ) ret.emplace_back( std::make_pair( obj.ip_address, obj.port ) );
    return ret;
}
#define l_class tcp_server
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( tcp_server
           , l_base( node )
           , l_set( create )
           , l_set( get_ip_datas )
    );
}
#include "lua_undef.h"
}