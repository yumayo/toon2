#pragma once
#include "../node.h"
namespace network
{
class udp_object : public node
{
protected:
    class _member;
    std::shared_ptr<_member> _m;
public:
    virtual void write( std::string const& message, std::function<void( )> on_send = nullptr ) = 0;
    virtual void write( char const* message, size_t size, std::function<void( )> on_send = nullptr ) = 0;
    void close( );
public:
    std::function<void( char const*, size_t )> on_readed;
    std::function<void( )> on_read_failed;
    std::function<void( )> on_closed;
    std::function<void( )> on_send_failed;
};
}