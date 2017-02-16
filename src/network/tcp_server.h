#pragma once
#include "../node.h"
#include "asio/error.hpp"
#include "client_handle.h"
namespace network
{
class tcp_server : public node
{
    struct _member;
    std::shared_ptr<_member> _m;
public:
    CREATE_H( tcp_server, std::string const& port, int num_of_client );
    ~tcp_server( );
    bool init( std::string const& port, int num_of_client );
    void update( float delta )override;
public:
    void write( std::string const& name, std::string const& message, std::function<void( )> on_send = nullptr );
    void write( std::string const& name, char const* message, size_t size, std::function<void( )> on_send = nullptr );
    void speech( std::string const& message, std::function<void( )> on_send = nullptr );
    void speech( char const* message, size_t size, std::function<void( )> on_send = nullptr );
    void close( std::string const& name );
public:
    std::function<void( )> on_startup_failed;
    std::function<void( client_handle const& )> on_handshake;
    std::function<void( )> on_connections_overflow;
    std::function<void( client_handle const& )> on_send_failed;
    std::function<void( client_handle const&, char const*, size_t )> on_readed;
    std::function<void( client_handle const& )> on_client_disconnected;
    std::function<void( client_handle const&, asio::error_code const& )> on_errored;
public:
    LUA_SETUP_H( tcp_server );
private:
    void lua_write_string_default( std::string const& name, std::string const& message );
    void lua_write_binary_default( std::string const& name, char const* message, size_t size );
    void lua_write_string( std::string const& name, std::string const& message, std::function<void( )> on_send );
    void lua_write_binary( std::string const& name, char const* message, size_t size, std::function<void( )> on_send );
    void lua_speech_string_default( std::string const& message );
    void lua_speech_binary_default( char const* message, size_t size );
    void lua_speech_string( std::string const& message, std::function<void( )> on_send );
    void lua_speech_binary( char const* message, size_t size, std::function<void( )> on_send );
    std::function<void( client_handle const&, int )> lua_on_errored;
};
}