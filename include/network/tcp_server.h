#pragma once
#include "node.h"
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
    void write( client_handle const& handle, std::string const& message, std::function<void( )> on_send = nullptr );
    void write( client_handle const& handle, char const* message, size_t size, std::function<void( )> on_send = nullptr );
    void speech( std::string const& message, std::function<void( )> on_send = nullptr );
    void speech( char const* message, size_t size, std::function<void( )> on_send = nullptr );
    void close( client_handle const& handle );
public:
    std::function<void( )> on_startup_failed;
    std::function<void( )> on_connections_overflow;
    std::function<void( client_handle const& )> on_send_failed;
    std::function<void( client_handle const&, char const*, size_t )> on_readed;
    std::function<void( client_handle const& )> on_client_disconnected;
    std::function<void( client_handle const&, asio::error_code const& )> on_errored;
};
}