#pragma once
#include "../node.h"
namespace network
{
class tcp_client : public node
{
    struct _member;
    std::shared_ptr<_member> _m;
public:
    CREATE_H( tcp_client, std::string const& ip_address, std::string const& port );
    bool init( std::string const& ip_address, std::string const& port );
    void connect( );
public:
    LUA_SETUP_H( tcp_client );
};
}