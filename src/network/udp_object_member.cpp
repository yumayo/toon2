#include "udp_object_member.h"
using namespace utility;
namespace network
{
udp_object::_member::_member( udp_object& parent, std::string const& port )
    : parent( parent )
    , socket( io, udp::endpoint( asio::ip::udp::v4( ), boost::lexical_cast<int>( port ) ) )
    , port( port )
{
    buffer.fill( 0 );
    recv_thread = std::make_unique<std::thread>( [ this ]
    {
        while ( socket.is_open( ) )
        {
            read( remote_endpoint_buffer );
        }
    } );
}
udp_object::_member::_member( udp_object & parent, std::string const & ip_address, std::string const & port )
    : parent( parent )
    , socket( io )
    , ip_address( ip_address )
    , port( port )
{
    buffer.fill( 0 );
    socket.open( udp::v4( ) );
    write( ip_address, port, asio::buffer( "" ), nullptr ); // 仮データを送信しないとその下のrecvにエラーが出ます。
    recv_thread = std::make_unique<std::thread>( [ this ]
    {
        while ( socket.is_open( ) )
        {
            read( socket.local_endpoint( ) );
        }
    } );
}
udp_object::_member::~_member( )
{
    close( );
    recv_thread->join( );
}
void udp_object::_member::read( udp::endpoint & sender_endpoint )
{
    try
    {
        auto bytes_transferred = socket.receive_from( asio::buffer( buffer ),
                                                      sender_endpoint );
        log( "【%s】受け取ったデータ: %d byte", class_name, bytes_transferred );
        log_data( buffer.data( ), bytes_transferred );
        if ( parent.on_readed ) parent.on_readed( buffer.data( ), bytes_transferred );
        std::fill_n( buffer.begin( ), bytes_transferred, 0 );
    }
    catch ( asio::error_code& e )
    {
        log( "【%s】データを受け取れませんでした。: %s", class_name, e.message( ).c_str( ) );
        if ( parent.on_read_failed ) parent.on_read_failed( );
    }
}
void udp_object::_member::write( std::string const& ip_address, std::string const& port, asio::const_buffers_1 buffer, std::function<void( )> on_send )
{
    try
    {
        udp::resolver resolver( io );
        udp::resolver::query query( udp::v4( ),
                                    ip_address,
                                    port );
        udp::endpoint receiver_endpoint = *resolver.resolve( query );
        auto len = socket.send_to( asio::buffer( buffer ), receiver_endpoint );
        // log( "【udp_object】送信中..." );
        if ( on_send ) on_send( );
    }
    catch ( asio::error_code& error )
    {
        log( "【%s】エラー: %s", class_name, error.message( ).c_str( ) );
        if ( parent.on_send_failed ) parent.on_send_failed( );
    }
}
void udp_object::_member::close( )
{
    if ( parent.on_closed ) parent.on_closed( );
    socket.close( );
}
}