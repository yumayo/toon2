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
    void log_data( socket_object& sock_obj, size_t bytes_transferred );
    bool is_max( );
    void close_message( std::string const& message, socket_object& sock_obj );
    asio::io_service io;
    std::shared_ptr<tcp::acceptor> acceptor;
    std::vector<socket_object> sockets;
    std::string port;
    std::set<connect_data> ip_data;
};
void tcp_server::_member::async_accept( socket_object& sock_obj )
{
    log( "【tcp_server】ソケットの準備" );
    acceptor->async_accept( sock_obj.socket, [ this, &sock_obj ] ( asio::error_code const& error )
    {
        if ( error )
        {
            log( "【tcp_server】接続は無効になりました。: %s", error.message( ).c_str( ) );
        }
        else
        {
            log( "【tcp_server】接続を受け付けました。" );
            if ( is_max( ) )
            {
                // 満員のときは即座に接続を切り、もう一度人数確認用に接続を受け付けます。
                close_message( "満員です。", sock_obj );
                async_accept( sock_obj );
            }
            else
            {
                log( "【tcp_server】接続成功！: %s, %d", sock_obj.socket.remote_endpoint( ).address( ).to_string( ).c_str( ), sock_obj.socket.remote_endpoint( ).port( ) );
                ip_data.insert( connect_data( sock_obj.socket.remote_endpoint( ).address( ).to_string( ),
                                              sock_obj.socket.remote_endpoint( ).port( ) ) );

                asio::async_read(
                    sock_obj.socket,
                    asio::buffer( sock_obj.buffer ),
                    asio::transfer_at_least( 1 ), // １バイトでもデータが送られてきたら、読み込みを開始します。
                    [ this, &sock_obj ] ( const asio::error_code& error, size_t bytes_transferred )
                {
                    if ( error )
                    {
                        if ( error == asio::error::eof )
                        {
                            log( "【tcp_server】クライアントが接続を切りました。: %s", error.message( ).c_str( ) );
                            ip_data.erase( connect_data( sock_obj.socket.remote_endpoint( ).address( ).to_string( ),
                                                         sock_obj.socket.remote_endpoint( ).port( ) ) );

                            // クライアントがいなくなったソケットは、もう一度接続します。
                            sock_obj.socket.close( );
                            async_accept( sock_obj );
                        }
                        else
                        {
                            log( "【tcp_server】無効なアクセスです。: %s", error.message( ).c_str( ) );
                        }
                    }
                    else
                    {
                        log_data( sock_obj, bytes_transferred );
                        std::fill_n( sock_obj.buffer.begin( ), bytes_transferred, 0 );

                        ip_data.erase( connect_data( sock_obj.socket.remote_endpoint( ).address( ).to_string( ),
                                                     sock_obj.socket.remote_endpoint( ).port( ) ) );

                        // クライアントがいなくなったソケットは、もう一度接続します。
                        sock_obj.socket.close( );
                        async_accept( sock_obj );
                    }
                } );
            }
        }
    } );
}
void tcp_server::_member::log_data( socket_object& sock_obj, size_t bytes_transferred )
{
    const char* data = sock_obj.buffer.data( );
    log( "【tcp_server】受け取ったデータ: %d byte", bytes_transferred );
    std::string output = ">[ ";
    size_t i;
    for ( i = 0; i < bytes_transferred - 1; ++i )
    {
        if ( ( i % 16 ) == 15 )
        {
            output += format( "%02X", data[i] & 0x000000FF );
            output += " ]\n>[ ";
        }
        else
        {
            if ( ( i % 4 ) == 3 )
            {
                output += format( "%02X | ", data[i] & 0x000000FF );
            }
            else
            {
                output += format( "%02X", data[i] & 0x000000FF );
            }
        }
    }
    output += format( "%02X", data[i] & 0x000000FF );
    log( "%s ]", output.c_str( ) );
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
void tcp_server::_member::close_message( std::string const & message, socket_object & sock_obj )
{
    asio::async_write( sock_obj.socket, asio::buffer( message.c_str( ), message.size( ) ),
                       [ ] ( const asio::error_code& error, size_t bytes_transferred )
    {
        if ( error )
        {
            log( "【tcp_server】データを送れませんでした。: %s", error.message( ).c_str( ) );
        }
        else
        {
            log( "【tcp_server】送信成功！" );
        }
    } );
    sock_obj.socket.close( );
}
CREATE_CPP( tcp_server, std::string const& port, int num_of_client )
{
    CREATE( tcp_server, port, num_of_client );
}
tcp_server::~tcp_server( )
{
    _m->io.stop( );
}
bool tcp_server::init( std::string const& port, int num_of_client )
{
    _m.reset( );
    _m = std::make_shared<_member>( port, num_of_client );

    set_schedule_update( );

    for ( int i = 0; i < num_of_client + 1; ++i )
    {
        _m->async_accept( _m->sockets[i] );
    }

    return true;
}
void tcp_server::update( float delta )
{
    _m->io.reset( );
    _m->io.poll( );
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