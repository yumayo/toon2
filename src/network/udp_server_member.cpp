#include "udp_server_member.h"
namespace network
{
udp_server::_member::_member( udp_server& parent, std::string const& port )
    : parent( parent )
    , socket( io, udp::endpoint( asio::ip::udp::v4( ), boost::lexical_cast<int>( port ) ) )
    , server_port( port )
{
    buffer.fill( 0 );
    read( );
}
udp_server::_member::~_member( )
{
    close( );
}
void udp_server::_member::update( )
{
    io.reset( );
    io.poll( );
}
void udp_server::_member::read( )
{
    socket.async_receive_from( asio::buffer( buffer ),
                               remote_endpoint_buffer,
                               [ this ] ( const asio::error_code& e, size_t bytes_transferred )
    {
        if ( e )
        {
            log( "【%s】データを受け取れませんでした。: %s", class_name, e.message( ).c_str( ) );
            if ( parent.on_read_failed ) parent.on_read_failed( );
        }
        else
        {
            log( "【%s】受け取ったデータ: %d byte", class_name, bytes_transferred );
            log_data( buffer.data( ), bytes_transferred );
            if ( parent.on_readed ) parent.on_readed( buffer.data( ), bytes_transferred );
            std::fill_n( buffer.begin( ), bytes_transferred, 0 );
            read( );
        }
    } );
}
void udp_server::_member::write( asio::const_buffers_1 buffer, std::function<void( )> on_send )
{
    if ( remote_endpoint_buffer.port( ) == 0 ) return;
    try
    {
        udp::resolver resolver( io );
        udp::resolver::query query( udp::v4( ),
                                    remote_endpoint_buffer.address( ).to_string( ),
                                    boost::lexical_cast<std::string>( remote_endpoint_buffer.port( ) ) );
        udp::endpoint receiver_endpoint = *resolver.resolve( query );
        auto len = socket.send_to( asio::buffer( buffer ), receiver_endpoint );
        // log( "【udp_server】送信中..." );
        if ( on_send ) on_send( );
    }
    catch ( asio::error_code& error )
    {
        log( "【%s】エラー: %s", class_name, error.message( ).c_str( ) );
        if ( parent.on_send_failed ) parent.on_send_failed( );
    }
}
void udp_server::_member::close( )
{
    if ( parent.on_closed ) parent.on_closed( );
    socket.close( );
}
}