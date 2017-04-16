#include "udp_connection_member.h"
#include "jsoncpp/json.h"
#include "utility/string_utility.h"
namespace network
{
udp_connection::member::member( udp_connection & server, udp::endpoint const & end_point )
    : _connection( server )
    , _udp_socket( _io_service, end_point )
    , _client_manager( server )
{
    _remote_buffer.fill( 0 );

    _receive( );

    _update_io_service = std::thread( [ this ]
    {
        while ( _is_update )
        {
            _io_service.run( );
            _io_service.reset( );
        }
    } );
}
udp_connection::member::member( udp_connection & server )
    : udp_connection::member( server, udp::endpoint( )/*空のendpointを送るとopenやlocal_endpointも設定してくれるようです。*/ )
{
}
udp_connection::member::member( udp_connection& server, int const & port_num )
    : udp_connection::member( server, udp::endpoint( udp::v4( ), port_num ) )
{
}
udp_connection::member::~member( )
{
    _kill( );
}
void udp_connection::member::write( network_handle const & handle, Json::Value const & send_data )
{
    write( handle, Json::FastWriter( ).write( send_data ) );
}
void udp_connection::member::write( network_handle const & handle, std::string const & send_data )
{
    write( handle, send_data.c_str( ), send_data.size( ) );
}
void udp_connection::member::write( network_handle const & handle, char const * send_data )
{
    write( handle, std::string( send_data ) );
}
void udp_connection::member::write( network_handle const & handle, char const * send_data, size_t const & send_data_byte )
{
    try
    {
        udp::resolver resolver( _io_service );
        udp::resolver::query query( udp::v4( ),
                                    handle->ip_address,
                                    boost::lexical_cast<std::string>( handle->port ) );
        _udp_socket.send_to( asio::buffer( send_data, send_data_byte ),
                             resolver.resolve( query )->endpoint( ) );
        utility::log_network( _remote_endpoint.address( ).to_string( ), _remote_endpoint.port( ),
                              "データを送信しました。" );
    }
    catch ( asio::error_code& error )
    {
        utility::log_network( _remote_endpoint.address( ).to_string( ), _remote_endpoint.port( ),
                              "データを送れませんでした。: %s", error.message( ).c_str( ) );
        if ( _connection.on_send_failed )_connection.on_send_failed( );
    }
    if ( _connection.on_sended )_connection.on_sended( );
}
void udp_connection::member::_kill( )
{
    if ( !_is_update ) return;
    _is_update = false;
    _io_service.stop( );
    _update_io_service.join( );
}
void udp_connection::member::update( float delta_second )
{
    _client_manager.update( delta_second );
}
void udp_connection::member::_receive( )
{
    _udp_socket.async_receive_from( asio::buffer( _remote_buffer ),
                                    _remote_endpoint,
                                    [ this ] ( const asio::error_code& e, size_t bytes_transferred )
    {
        if ( e )
        {
            utility::log_network( _remote_endpoint.address( ).to_string( ), _remote_endpoint.port( ),
                                  "データを受け取れませんでした。: %s", e.message( ).c_str( ) );
            if ( _connection.on_read_failed ) _connection.on_read_failed( );
        }
        else
        {
            utility::log_network( _remote_endpoint.address( ).to_string( ), _remote_endpoint.port( ),
                                  "データを受信しました。" );

            auto handle = _client_manager.regist( _remote_endpoint.address( ).to_string( ),
                                                  _remote_endpoint.port( ) );
            handle->timeout_restart( );

            // データを受け取ったら呼び出されます。
            if ( _connection.on_received ) _connection.on_received( handle,
                                                                    _remote_buffer.data( ), bytes_transferred );

            Json::Value root;
            if ( Json::Reader( ).parse( std::string( _remote_buffer.data( ), bytes_transferred ), root ) )
            {
                // 通常用のjson関数を呼び出します。
                if ( _connection.on_received_json )_connection.on_received_json( handle, root );
                
                // map式のjson関数を呼び出します。
                auto itr = _connection.on_received_named_json.find( root["name"].asString( ) );
                if ( itr != std::end( _connection.on_received_named_json ) )
                {
                    if ( itr->second ) itr->second( handle, root );
                }
            }

            std::fill_n( _remote_buffer.begin( ), bytes_transferred, 0 );

            // 再帰呼び出しですが、非同期なので大丈夫です。
            _receive( );
        }
    } );
}
utility::recursion_usable_mutex & udp_connection::member::get_mutex( )
{
    return _mutex;
}
}
