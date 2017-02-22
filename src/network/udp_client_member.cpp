#include "udp_client_member.h"
namespace network
{
udp_client::_member::_member( udp_client & parent, std::string const & ip_address, std::string const & port )
    : parent( parent )
    , socket( io )
    , ip_address( ip_address )
    , port( port )
{
    buffer.fill( 0 );
    socket.open( udp::v4( ) );
    // 空データを送信しないとlocal_endpointが記入されないので。
    write( asio::buffer( "" ), nullptr ); 
    read( );
}
udp_client::_member::~_member( )
{
    close( );
}
void udp_client::_member::update( )
{
    io.reset( );
    io.poll( );
}
void udp_client::_member::read( )
{
    socket.async_receive_from( asio::buffer( buffer ),
                               socket.local_endpoint( ),
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
        log( "【%s】エラー: %s", class_name, error.message( ).c_str( ) );
        if ( parent.on_send_failed ) parent.on_send_failed( );
    }
}
void udp_client::_member::close( )
{
    if ( parent.on_closed ) parent.on_closed( );
    socket.close( );
}
}