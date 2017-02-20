#pragma once
#include "../node.h"
namespace network
{
class udp_reader : public node
{
    struct _member;
    std::shared_ptr<_member> _m;
public:
    CREATE_H( udp_reader, std::string const& port );
    bool init( std::string const& port );
    void update( float delta ) override;
public:
    void close( );
public:
    std::function<void( char const*, size_t )> on_readed;
    std::function<void( )> on_read_failed;
    std::function<void( )> on_closed;
};
}