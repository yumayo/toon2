#include "tcp_server.h"
#ifndef ASIO_HAS_STD_ATOMIC
#define ASIO_HAS_STD_ATOMIC
#endif
#define ASIO_HAS_BOOST_DATE_TIME
#define BOOST_DATE_TIME_NO_LIB
#include "asio/asio.hpp"
using asio::ip::tcp;
#include "boost/lexical_cast.hpp"
#include "boost/bind.hpp"
#include "utility/assert_log.h"
#include "client_handle.h"
#include <set>
#include "utility/utf8.h"
using namespace utility;
namespace network
{
class socket_object
{
public:
    socket_object( ) = delete;
    socket_object( socket_object const& ) = delete;
    socket_object( asio::io_service& io )
        : socket( io )
        , handle( _ip_address, _port )
    {
        buffer.fill( 0 );
    }
    void close( )
    {
        _ip_address = "";
        _port = "";
        socket.close( );
        buffer.fill( 0 );
    }
    void connect( std::string const& ip_address, std::string const& port )
    {
        _ip_address = ip_address;
        _port = port;
    }
    void buffer_clear( size_t bytes_transferred )
    {
        std::fill_n( buffer.begin( ), bytes_transferred, 0 );
    }
    char const* c_str( )
    {
        return buffer.data( );
    }
    tcp::socket socket;
    boost::array<char, 512> buffer;
    client_handle handle;
private: // 以下の値をハンドルに詰め込んで運びます。
    std::string _ip_address;
    std::string _port;
};
struct tcp_server::_member
{
    _member( tcp_server& parent, std::string const& port, int num_of_client )
        : parent( parent )
        , io( )
        , acceptor( )
        , sockets( )
        , port( port )
    {
        assert_log( 1 <= num_of_client, "クライアントの数は一つ以上でないといけません。", return );
        acceptor = std::make_unique<tcp::acceptor>( io, tcp::endpoint( tcp::v4( ), boost::lexical_cast<int>( port ) ) );
        // 一つはクライアントの接続を切る役割を持たせています。
        for ( int i = 0; i < num_of_client + 1; ++i )
        {
            sockets.emplace_back( std::make_unique<socket_object>( io ) );
        }
        int a = 0;
    }
    void async_accept( socket_object& sock_obj );
    bool is_max( );
    void write( socket_object& sock_obj, asio::const_buffers_1 buffer, std::function<void( )> on_send );
    void read( socket_object& sock_obj );
    void on_errored( socket_object& sock_obj, asio::error_code const& e );
    void close_with_async( socket_object& sock_obj );
    void find_run( client_handle const& handle, std::function<void( socket_object& )> call );
    tcp_server& parent;
    asio::io_service io;
    std::unique_ptr<tcp::acceptor> acceptor;
    std::vector<std::unique_ptr<socket_object>> sockets;
    std::string port;
};
void tcp_server::_member::async_accept( socket_object& sock_obj )
{
    log( "【tcp_server】ソケットの準備" );
    acceptor->async_accept( sock_obj.socket, [ this, &sock_obj ] ( asio::error_code const& e )
    {
        if ( e )
        {
            log( "【tcp_server】接続は無効になりました。: %s", e.message( ).c_str( ) );
            if ( parent.on_startup_failed ) parent.on_startup_failed( );
        }
        else
        {
            log( "【tcp_server】接続を受け付けました。" );
            if ( is_max( ) )
            {
                if ( parent.on_connections_overflow ) parent.on_connections_overflow( );
                close_with_async( sock_obj );
            }
            else
            {
                log( "【tcp_server】接続成功！: %s, %d", sock_obj.socket.remote_endpoint( ).address( ).to_string( ).c_str( ), sock_obj.socket.remote_endpoint( ).port( ) );
                sock_obj.connect( sock_obj.socket.remote_endpoint( ).address( ).to_string( ), boost::lexical_cast<std::string>( sock_obj.socket.remote_endpoint( ).port( ) ) );
                read( sock_obj );
            }
        }
    } );
}
bool tcp_server::_member::is_max( )
{
    int num = 0;
    for ( auto& obj : sockets )
    {
        if ( obj->socket.is_open( ) ) num++;
    }
    return sockets.size( ) == num;
}
void tcp_server::_member::read( socket_object & sock_obj )
{
    asio::async_read(
        sock_obj.socket,
        asio::buffer( sock_obj.buffer ),
        asio::transfer_at_least( 1 ), // １バイトでもデータが送られてきたら、読み込みを開始します。
        [ this, &sock_obj ] ( const asio::error_code& e, size_t bytes_transferred )
    {
        if ( e )
        {
            if ( e == asio::error::eof )
            {
                log( "【tcp_server】クライアントが接続を切りました。: %s", e.message( ).c_str( ) );
                if ( parent.on_client_disconnected ) parent.on_client_disconnected( sock_obj.handle );
            }
            else
            {
                log( "【tcp_server】無効なアクセスです。: %s", e.message( ).c_str( ) );
                on_errored( sock_obj, e );
            }
            close_with_async( sock_obj );
        }
        else
        {
            log( "【tcp_server】受け取ったデータ: %d byte", bytes_transferred );
            log_data( sock_obj.buffer.data( ), bytes_transferred );
            if ( parent.on_readed ) parent.on_readed( sock_obj.handle, sock_obj.buffer.data( ), bytes_transferred );
            std::fill_n( sock_obj.buffer.begin( ), bytes_transferred, 0 );

            // クライアントからの接続が切れるまで無限に受け取り続けます。
            read( sock_obj );
        }
    } );
}
void tcp_server::_member::write( socket_object& sock_obj, asio::const_buffers_1 buffer, std::function<void( )> on_send )
{
    asio::async_write(
        sock_obj.socket,
        buffer,
        [ this, &sock_obj, on_send, buffer ] ( const asio::error_code& e, size_t bytes_transferred )
    {
        if ( e )
        {
            log( "【tcp_server】送信できませんでした。: %s", e.message( ).c_str( ) );
            if ( parent.on_send_failed ) parent.on_send_failed( sock_obj.handle );
        }
        else
        {
            log( "【tcp_server】送信成功！" );
            if ( on_send ) on_send( );
        }
    } );
}
void tcp_server::_member::on_errored( socket_object& sock_obj, asio::error_code const & e )
{
    if ( parent.on_errored ) parent.on_errored( sock_obj.handle, e );
}
void tcp_server::_member::close_with_async( socket_object & sock_obj )
{
    // クライアントがいなくなったソケットは、もう一度接続します。
    sock_obj.close( );
    async_accept( sock_obj );
}
void tcp_server::_member::find_run( client_handle const& handle, std::function<void( socket_object& )> call )
{
    auto itr = std::find_if( std::begin( sockets ), std::end( sockets ), [ handle ] ( std::unique_ptr<socket_object>& object )
    {
        return object->handle == handle;
    } );
    if ( itr != sockets.end( ) )
    {
        call( **itr );
    }
    else
    {
        log( "【tcp_server】名前と一致するクライアントが見つかりませんでした。" );
    }
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
    _m = std::make_shared<_member>( *this, port, num_of_client );

    set_schedule_update( );

    for ( int i = 0; i < num_of_client + 1; ++i )
    {
        _m->async_accept( *_m->sockets[i] );
    }

    return true;
}
void tcp_server::update( float delta )
{
    _m->io.reset( );
    _m->io.poll( );
}
void tcp_server::write( client_handle const& handle, std::string const & message, std::function<void( )> on_send )
{
    write( handle, message.c_str( ), message.size( ), on_send );
}
void tcp_server::write( client_handle const& handle, char const * message, size_t size, std::function<void( )> on_send )
{
    _m->find_run( handle, [ this, message, size, on_send ] ( socket_object& sock_obj )
    {
        _m->write( sock_obj, asio::buffer( message, size ), on_send );
    } );
}
void tcp_server::speech( std::string const & message, std::function<void( )> on_send )
{
    speech( message.c_str( ), message.size( ), on_send );
}
void tcp_server::speech( char const * message, size_t size, std::function<void( )> on_send )
{
    for ( auto& obj : _m->sockets )
    {
        _m->write( *obj, asio::buffer( message, size ), on_send );
    }
}
void tcp_server::close( client_handle const& handle )
{
    _m->find_run( handle, [ this ] ( socket_object& sock_obj )
    {
        _m->close_with_async( sock_obj );
    } );
}
}