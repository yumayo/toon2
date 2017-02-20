﻿#include "udp_client.h"
#define ASIO_HAS_STD_ATOMIC
#define ASIO_HAS_BOOST_DATE_TIME
#define BOOST_DATE_TIME_NO_LIB
#include "asio/asio.hpp"
using udp = asio::ip::udp;
#include "boost/array.hpp"
#include "../utility/string_utility.h"
#include "boost/lexical_cast.hpp"
using namespace utility;
namespace network
{
class udp_client::_member
{
public:
    _member( udp_client& parent, std::string const & ip_address, std::string const & port );
    ~_member( );
    udp_client& parent;

    asio::io_service io;
    udp::socket socket;

    std::string port; // サーバーのポート番号
    std::string ip_address; // サーバーのIPアドレス

    udp::endpoint remote_endpoint_buffer; // 接続してきた相手の情報が一時的に保存されます。

    std::unique_ptr<std::thread> recv_thread;

    boost::array<char, 512> buffer;

    void read( );
    void write( asio::const_buffers_1 buffer, std::function<void( )> on_send );
    void close( );
};
CREATE_CPP( udp_client, std::string const & ip_address, std::string const & port )
{
    CREATE( udp_client, ip_address, port );
}
bool udp_client::init( std::string const & ip_address, std::string const & port )
{
    _m = std::make_shared<_member>( *this, ip_address, port );
    return true;
}
void udp_client::write( std::string const & message, std::function<void( )> on_send )
{
    _m->write( asio::buffer( message.c_str( ), message.size( ) ), on_send );
}
void udp_client::write( char const * message, size_t size, std::function<void( )> on_send )
{
    _m->write( asio::buffer( message, size ), on_send );
}
void udp_client::close( )
{
    _m->close( );
}
udp_client::_member::_member( udp_client& parent, std::string const & ip_address, std::string const & port )
    : parent( parent )
    , socket( io )
    , ip_address( ip_address )
    , port( port )
{
    buffer.fill( 0 );
    socket.open( udp::v4( ) );
    write( asio::buffer( "" ), nullptr ); // 仮データを送信しないとその下のrecvにエラーが出ます。
    recv_thread = std::make_unique<std::thread>( [ this ]
    {
        while ( socket.is_open( ) )
        {
            read( );
        }
    } );
}
udp_client::_member::~_member( )
{
    close( );
    recv_thread->join( );
}
void udp_client::_member::read( )
{
    try
    {
        auto bytes_transferred = socket.receive_from( asio::buffer( buffer ),
                                                      socket.local_endpoint( ) );
        log( "【udp_client】受け取ったデータ: %d byte", bytes_transferred );
        log_data( buffer.data( ), bytes_transferred );
        if ( parent.on_readed ) parent.on_readed( buffer.data( ), bytes_transferred );
        std::fill_n( buffer.begin( ), bytes_transferred, 0 );
    }
    catch ( asio::error_code& e )
    {
        log( "【udp_client】データを受け取れませんでした。: %s", e.message( ).c_str( ) );
        if ( parent.on_read_failed ) parent.on_read_failed( );
    }
}
void udp_client::_member::write( asio::const_buffers_1 buffer, std::function<void( )> on_send )
{
    try
    {
        udp::resolver resolver( io );
        udp::resolver::query query( udp::v4( ),
                                    ip_address,
                                    port );
        udp::endpoint receiver_endpoint = *resolver.resolve( query );
        auto len = socket.send_to( asio::buffer( buffer ), receiver_endpoint );
        // log( "【udp_client】送信中..." );
        if ( on_send ) on_send( );
    }
    catch ( asio::error_code& error )
    {
        log( "【udp_client】エラー: %s", error.message( ).c_str( ) );
        if ( parent.on_send_failed ) parent.on_send_failed( );
    }
}
void udp_client::_member::close( )
{
    if ( parent.on_closed ) parent.on_closed( );
    socket.close( );
}
}
