#include "tcp_client.h"
#define ASIO_HAS_STD_ATOMIC
#define ASIO_HAS_BOOST_DATE_TIME
#define BOOST_DATE_TIME_NO_LIB
#include "asio/asio.hpp"
using asio::ip::tcp;
#include "boost/lexical_cast.hpp"
#include "boost/bind.hpp"
#include "../utility/assert_log.h"
using namespace utility;
namespace network
{
struct tcp_client::_member
{
    _member( tcp_client& parent, std::string const& ip_address, std::string const& port )
        : parent( parent )
        , io( )
        , socket( io )
        , ip_address( ip_address )
        , port( port )
    {
        buffer.fill( 0 );
    }
    tcp_client& parent;
    void connect( );
    void write( asio::const_buffers_1 buffer, std::function<void( )> on_send );
    void read( );
    void handshake( );
    void sys_close( );
    void sys_error( asio::error_code const& error );
    asio::io_service io;
    tcp::socket socket;
    boost::array<char, 512> buffer;
    std::string ip_address;
    std::string port;
};
void tcp_client::_member::connect( )
{
    socket.async_connect(
        tcp::endpoint( asio::ip::address::from_string( ip_address ), boost::lexical_cast<int>( port ) ),
        [ this ] ( const asio::error_code& error )
    {
        if ( error )
        {
            log( "【tcp_client】接続できませんでした。: %s", error.message( ).c_str( ) );
            if ( parent.on_connect_failed ) parent.on_connect_failed( );
            sys_close( );
        }
        else
        {
            log( "【tcp_client】接続成功！" );
            // 接続が成功したらハンドシェイクを試みます。
            handshake( );
        }
    } );
}
void tcp_client::_member::write( asio::const_buffers_1 buffer, std::function<void( )> on_send )
{
    asio::async_write(
        socket,
        buffer,
        [ this, on_send, buffer ] ( const asio::error_code& error, size_t bytes_transferred )
    {
        if ( error )
        {
            log( "【tcp_client】送信できませんでした。: %s", error.message( ).c_str( ) );
            if ( parent.on_send_failed ) parent.on_send_failed( );
            sys_close( );
        }
        else
        {
            log( "【tcp_client】送信成功！" );
            if ( on_send ) on_send( );
        }
    } );
}
void tcp_client::_member::read( )
{
    asio::async_read(
        socket,
        asio::buffer( buffer ),
        asio::transfer_at_least( 1 ),
        [ this ] ( const asio::error_code& error, size_t bytes_transferred )
    {
        if ( error )
        {
            if ( error == asio::error::eof )
            {
                log( "【tcp_client】サーバーが接続を切りました。: %s", error.message( ).c_str( ) );
                if ( parent.on_disconnected ) parent.on_disconnected( );
            }
            else
            {
                log( "【tcp_client】無効なアクセスです。: %s", error.message( ).c_str( ) );
                sys_error( error );
            }
            sys_close( );
        }
        else
        {
            log( "【tcp_client】受け取ったデータ: %d byte", bytes_transferred );
            log_data( buffer.data( ), bytes_transferred );
            if ( parent.on_readed ) parent.on_readed( buffer.data( ), bytes_transferred );
            std::fill_n( buffer.begin( ), bytes_transferred, 0 );

            // エラーじゃない限り無限に受け取りを続けます。
            read( );
        }
    } );
}
void tcp_client::_member::handshake( )
{
    asio::async_write(
        socket,
        asio::buffer( parent._name.c_str( ), parent._name.size( ) ),
        [ this ] ( const asio::error_code& error, size_t bytes_transferred )
    {
        if ( error )
        {
            log( "【tcp_client】ハンドシェイクできませんでした。: %s", error.message( ).c_str( ) );
            if ( parent.on_handshake_faild ) parent.on_handshake_faild( );
            sys_close( );
        }
        else
        {
            log( "【tcp_client】ハンドシェイク成功！" );
            // ハンドシェイクに成功した場合は、サーバーからの読み込みを開始します。
            read( );
        }
    } );
}
void tcp_client::_member::sys_close( )
{
    if ( parent.on_closed ) parent.on_closed( );
    socket.close( );
}
void tcp_client::_member::sys_error( asio::error_code const& error )
{
    if ( parent.on_errored ) parent.on_errored( error );
    if ( parent.lua_on_errored ) parent.lua_on_errored( error.value( ) );
}
CREATE_CPP( tcp_client, std::string const& ip_address, std::string const& port )
{
    CREATE( tcp_client, ip_address, port );
}
bool tcp_client::init( std::string const& ip_address, std::string const& port )
{
    _m.reset( );
    _m = std::make_shared<_member>( *this, ip_address, port );

    set_schedule_update( );

    _m->connect( );

    return true;
}
tcp_client::~tcp_client( )
{
    _m->sys_close( );
    _m->io.stop( );
}
void tcp_client::update( float delta )
{
    _m->io.reset( );
    _m->io.poll( );
}
void tcp_client::write( std::string const & message, std::function<void( )> on_send )
{
    write( message.c_str( ), message.size( ), on_send );
}
void tcp_client::write( char const * message, size_t size, std::function<void( )> on_send )
{
    log( "【tcp_client】送信中..." );
    _m->write( asio::buffer( message, size ), on_send );
}
void tcp_client::lua_write_string_default( std::string const & message )
{
    write( message );
}
void tcp_client::lua_write_binary_default( char const * message, size_t size )
{
    write( message, size );
}
void tcp_client::lua_write_string( std::string const & message, std::function<void( )> on_send )
{
    write( message, on_send );
}
void tcp_client::lua_write_binary( char const * message, size_t size, std::function<void( )> on_send )
{
    write( message, size, on_send );
}
#define l_class tcp_client
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( tcp_client
           , l_base( node )
           , l_set( create )
           , l_set( on_send_failed )
           , l_set( on_readed )
           , l_set( on_connect_failed )
           , l_set( on_disconnected )
           , l_set( on_closed )
           , "write", sol::overload( &l_class::lua_write_string_default,
                                     &l_class::lua_write_binary_default,
                                     &l_class::lua_write_string,
                                     &l_class::lua_write_binary )
           , "on_errored", &l_class::lua_on_errored
    );
}
#include "lua_undef.h"
}