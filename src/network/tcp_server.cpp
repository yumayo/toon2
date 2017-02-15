#include "tcp_server.h"
#define ASIO_HAS_BOOST_DATE_TIME
#define BOOST_DATE_TIME_NO_LIB
#include "asio/asio.hpp"
using asio::ip::tcp;
#include "boost/lexical_cast.hpp"
#include "boost/bind.hpp"
#include "../utility/assert_log.h"
#include "client_handle.h"
#include "network.hpp"
#include <set>
#include "boost/range/algorithm/find_if.hpp"
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
    void close( )
    {
        handle.clear( );
        socket.close( );
        buffer.fill( 0 );
    }
    client_handle handle;
    tcp::socket socket;
    boost::array<char, 512> buffer;
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
        acceptor = std::make_shared<tcp::acceptor>( io, tcp::endpoint( tcp::v4( ), boost::lexical_cast<int>( port ) ) );
        // 一つはクライアントの接続を切る役割を持たせています。
        for ( int i = 0; i < num_of_client + 1; ++i )
        {
            sockets.emplace_back( io );
        }
    }
    void async_accept( socket_object& sock_obj );
    bool is_max( );
    void write( socket_object& sock_obj, asio::const_buffers_1 buffer, std::function<void( )> on_send );
    void read( socket_object& sock_obj );
    void on_errored( asio::error_code const& error );
    std::vector<socket_object>::iterator find_socket_object( std::string const& name );
    tcp_server& parent;
    asio::io_service io;
    std::shared_ptr<tcp::acceptor> acceptor;
    std::vector<socket_object> sockets;
    std::string port;
};
void tcp_server::_member::async_accept( socket_object& sock_obj )
{
    log( "【tcp_server】ソケットの準備" );
    acceptor->async_accept( sock_obj.socket, [ this, &sock_obj ] ( asio::error_code const& error )
    {
        if ( error )
        {
            log( "【tcp_server】接続は無効になりました。: %s", error.message( ).c_str( ) );
            if ( parent.on_startup_failed ) parent.on_startup_failed( );
        }
        else
        {
            log( "【tcp_server】接続を受け付けました。" );
            if ( is_max( ) )
            {
                sock_obj.close( );
                if ( parent.on_maxed ) parent.on_maxed( );

                async_accept( sock_obj );
            }
            else
            {
                log( "【tcp_server】接続成功！: %s, %d", sock_obj.socket.remote_endpoint( ).address( ).to_string( ).c_str( ), sock_obj.socket.remote_endpoint( ).port( ) );
                if ( parent.on_connected ) parent.on_connected( );
                sock_obj.handle.connect( sock_obj.socket.remote_endpoint( ).address( ).to_string( ), sock_obj.socket.remote_endpoint( ).port( ) );

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
        if ( obj.socket.is_open( ) ) num++;
    }
    return sockets.size( ) == num;
}
void tcp_server::_member::read( socket_object & sock_obj )
{
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
                if ( parent.on_client_disconnected ) parent.on_client_disconnected( );
                sock_obj.close( );

                // クライアントがいなくなったソケットは、もう一度接続します。
                async_accept( sock_obj );
            }
            else
            {
                log( "【tcp_server】無効なアクセスです。: %s", error.message( ).c_str( ) );
                if ( parent.on_errored ) parent.on_errored( error );
            }
        }
        else
        {
            log_data( sock_obj.buffer.data( ), bytes_transferred );
            if ( parent.on_readed ) parent.on_readed( sock_obj.buffer.data( ), bytes_transferred );
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
        [ this, on_send, buffer ] ( const asio::error_code& error, size_t bytes_transferred )
    {
        if ( error )
        {
            log( "【tcp_client】送信できませんでした。: %s", error.message( ).c_str( ) );
            if ( parent.on_send_failed ) parent.on_send_failed( );
        }
        else
        {
            log( "【tcp_client】送信成功！" );
            if ( on_send ) on_send( );
        }
    } );
}
void tcp_server::_member::on_errored( asio::error_code const & error )
{
    if ( parent.on_errored ) parent.on_errored( error );
    if ( parent.lua_on_errored ) parent.lua_on_errored( error.value( ) );
}
std::vector<socket_object>::iterator tcp_server::_member::find_socket_object( std::string const & name )
{
    return boost::find_if( sockets, [ name ] ( socket_object& obj )
    {
        return obj.handle == name;
    } );
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
        _m->async_accept( _m->sockets[i] );
    }

    return true;
}
void tcp_server::update( float delta )
{
    _m->io.reset( );
    _m->io.poll( );
}
void tcp_server::write( std::string const & name, std::string const & message, std::function<void( )> on_send )
{
    write( name, message.c_str( ), message.size( ), on_send );
}
void tcp_server::write( std::string const & name, char const * message, size_t size, std::function<void( )> on_send )
{
    auto itr = _m->find_socket_object( name );
    if ( itr != _m->sockets.end( ) )
    {
        _m->write( *itr, asio::buffer( message, size ), on_send );
    }
}
void tcp_server::speech( std::string const & message, std::function<void( )> on_send )
{
    speech( message.c_str( ), message.size( ), on_send );
}
void tcp_server::speech( char const * message, size_t size, std::function<void( )> on_send )
{
    for ( auto& obj : _m->sockets )
    {
        _m->write( obj, asio::buffer( message, size ), on_send );
    }
}
void tcp_server::lua_write_string_default( std::string const & name, std::string const & message )
{
    write( name, message );
}
void tcp_server::lua_write_binary_default( std::string const & name, char const * message, size_t size )
{
    write( name, message, size );
}
void tcp_server::lua_write_string( std::string const & name, std::string const & message, std::function<void( )> on_send )
{
    write( name, message, on_send );
}
void tcp_server::lua_write_binary( std::string const & name, char const * message, size_t size, std::function<void( )> on_send )
{
    write( name, message, size, on_send );
}
void tcp_server::lua_speech_string_default( std::string const & message )
{
    speech( message );
}
void tcp_server::lua_speech_binary_default( char const * message, size_t size )
{
    speech( message, size );
}
void tcp_server::lua_speech_string( std::string const & message, std::function<void( )> on_send )
{
    speech( message, on_send );
}
void tcp_server::lua_speech_binary( char const * message, size_t size, std::function<void( )> on_send )
{
    speech( message, size, on_send );
}
#define l_class tcp_server
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( tcp_server
           , l_base( node )
           , l_set( create )
           , l_set( on_startup_failed )
           , l_set( on_connected )
           , l_set( on_maxed )
           , l_set( on_send_failed )
           , l_set( on_readed )
           , l_set( on_client_disconnected )
           , l_set( on_cannot_sended )
           , "write", sol::overload( &l_class::lua_write_string_default,
                                     &l_class::lua_write_binary_default,
                                     &l_class::lua_write_string,
                                     &l_class::lua_write_binary )
           , "speech", sol::overload( &l_class::lua_speech_string_default,
                                      &l_class::lua_speech_binary_default,
                                      &l_class::lua_speech_string,
                                      &l_class::lua_speech_binary )
           , "on_errored", &l_class::lua_on_errored
    );
}
#include "lua_undef.h"
}