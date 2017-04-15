#include "tcp_client.h"
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
    void close( );
    void error( asio::error_code const& error );
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
        [ this ] ( const asio::error_code& e )
    {
        if ( e )
        {
            log( "【tcp_client】接続できませんでした。: %s", e.message( ).c_str( ) );
            if ( parent.on_connect_failed ) parent.on_connect_failed( );
            close( );
        }
        else
        {
            log( "【tcp_client】接続成功！" );
            read( );
        }
    } );
}
void tcp_client::_member::write( asio::const_buffers_1 buffer, std::function<void( )> on_send )
{
    asio::async_write(
        socket,
        buffer,
        [ this, on_send, buffer ] ( const asio::error_code& e, size_t bytes_transferred )
    {
        if ( e )
        {
            log( "【tcp_client】送信できませんでした。: %s", e.message( ).c_str( ) );
            if ( parent.on_send_failed ) parent.on_send_failed( );
            close( );
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
        [ this ] ( const asio::error_code& e, size_t bytes_transferred )
    {
        if ( e )
        {
            if ( e == asio::error::eof )
            {
                log( "【tcp_client】サーバーが接続を切りました。: %s", e.message( ).c_str( ) );
                if ( parent.on_disconnected ) parent.on_disconnected( );
            }
            else
            {
                log( "【tcp_client】無効なアクセスです。: %s", e.message( ).c_str( ) );
                error( e );
            }
            close( );
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
void tcp_client::_member::close( )
{
    if ( parent.on_closed ) parent.on_closed( );
    socket.close( );
}
void tcp_client::_member::error( asio::error_code const& e )
{
    if ( parent.on_errored ) parent.on_errored( e );
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
    _m->close( );
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
}