#pragma once
#include "../node.h"
namespace network
{
class udp_server : public node
{
    class _member;
    std::shared_ptr<_member> _m;
public:
    CREATE_H( udp_server, std::string const& port );
    bool init( std::string const& port );
    void update( float delta ) override;
public:
    void write( std::string const& message, std::function<void( )> on_send = nullptr );
    void write( char const* message, size_t size, std::function<void( )> on_send = nullptr );
    void close( );
public:
    std::function<void( char const*, size_t )> on_readed;
    std::function<void( )> on_read_failed;
    std::function<void( )> on_closed;
    std::function<void( )> on_send_failed;
};
}