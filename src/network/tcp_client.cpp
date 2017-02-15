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
    _member( tcp_client& parent, std::string const& ip_address, std::string const& port, std::function<void( std::string const& )>& on_readed_call )
        : parent( parent )
        , io( )
        , socket( io )
        , ip_address( ip_address )
        , port( port )
        , on_readed_call( on_readed_call )
    {
        buffer.fill( 0 );
    }
    tcp_client& parent;
    void connect( );
    void write( asio::const_buffers_1 buffer, std::function<void( bool )> on_writed_call );
    void read( );
    asio::io_service io;
    tcp::socket socket;
    boost::array<char, 512> buffer;
    std::function<void( std::string const& )>& on_readed_call;
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
            parent.remove_from_parent( );
        }
        else
        {
            log( "【tcp_client】接続成功！" );
            read( );
        }
    } );
}
void tcp_client::_member::write( asio::const_buffers_1 buffer, std::function<void( bool )> on_writed_call )
{
    asio::async_write(
        socket,
        buffer,
        [ this, on_writed_call, buffer ] ( const asio::error_code& error, size_t bytes_transferred )
    {
        bool e = false;
        if ( error )
        {
            log( "【tcp_client】送信できませんでした。: %s", error.message( ).c_str( ) );
            e = true;
        }
        else
        {
            log( "【tcp_client】送信成功！" );
        }
        if ( on_writed_call ) on_writed_call( e );
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
        const char* data = nullptr;
        if ( error )
        {
            if ( error == asio::error::eof )
            {
                log( "【tcp_client】サーバーが接続を切りました。: %s", error.message( ).c_str( ) );
                parent.remove_from_parent( );
            }
            else
            {
                log( "【tcp_client】無効なアクセスです。: %s", error.message( ).c_str( ) );
            }
        }
        else
        {
            data = buffer.data( );
            log( "データ: %s", data );
            if ( on_readed_call ) on_readed_call( data );
            std::fill_n( buffer.begin( ), bytes_transferred, 0 );

            // エラーじゃない限り無限に受け取りを続けます。
            read( );
        }
    } );
}
CREATE_CPP( tcp_client, std::string const& ip_address, std::string const& port )
{
    CREATE( tcp_client, ip_address, port );
}
bool tcp_client::init( std::string const& ip_address, std::string const& port )
{
    _m.reset( );
    _m = std::make_shared<_member>( *this, ip_address, port, on_readed_call );

    set_schedule_update( );

    _m->connect( );

    return true;
}
tcp_client::~tcp_client( )
{
    _m->socket.close( );
    _m->io.stop( );
}
void tcp_client::update( float delta )
{
    _m->io.reset( );
    _m->io.poll( );
}
void tcp_client::write( std::string const & message, std::function<void( bool )> on_writed_call )
{
    write( message.c_str( ), message.size( ), on_writed_call );
}
void tcp_client::write( char const * message, size_t size, std::function<void( bool )> on_writed_call )
{
    log( "【tcp_client】送信中..." );
    _m->write( asio::buffer( message, size ), on_writed_call );
}
void tcp_client::write_string( std::string const & message, std::function<void( bool )> on_writed_call )
{
    write( message, on_writed_call );
}
void tcp_client::write_binary( char const * message, size_t size, std::function<void( bool )> on_writed_call )
{
    write( message, size, on_writed_call );
}
#define l_class tcp_client
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( tcp_client
           , l_base( node )
           , l_set( create )
           , "write", sol::overload( &tcp_client::write_string, &tcp_client::write_binary )
    );
}
#include "lua_undef.h"
}