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
    _member( std::string const& ip_address, std::string const& port )
        : io( )
        , socket( io )
        , ip_address( ip_address )
        , port( port )
    {
        buffer.fill( 0 );
    }
    void connect( );
    asio::io_service io;
    tcp::socket socket;
    boost::array<char, 512> buffer;
    std::shared_ptr<std::thread> thread;
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
            log( "接続できませんでした。: %s", error.message( ).c_str( ) );
        }
        else
        {
            log( "接続成功！" );

            asio::async_write(
                socket,
                asio::buffer( "hello" ),
                [ this ] ( const asio::error_code& error, size_t bytes_transferred )
            {
                if ( error )
                {
                    log( "送信できませんでした。: %s", error.message( ).c_str( ) );
                }
                else
                {
                    log( "送信成功！" );
                }
            } );
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
                        log( "サーバーが接続を切りました。: %s", error.message( ).c_str( ) );
                    }
                    else
                    {
                        log( "無効なアクセスです。: %s", error.message( ).c_str( ) );
                    }
                }
                else
                {
                    const char* data = buffer.data( );
                    log( "データ: %s", data );
                    std::fill_n( buffer.begin( ), bytes_transferred, 0 );
                    socket.close( );
                }
            } );
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
    _m = std::make_shared<_member>( ip_address, port );

    _m->thread = std::make_shared<std::thread>( [ this ]
    {
        _m->connect( );
        _m->io.run( );
    } );

    return true;
}
tcp_client::~tcp_client( )
{
    _m->io.stop( );
    _m->thread->join( );
}
#define l_class tcp_client
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( tcp_client
           , l_base( node )
           , l_set( create )
    );
}
#include "lua_undef.h"
}