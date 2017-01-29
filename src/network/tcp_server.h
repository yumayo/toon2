#pragma once
#include "../node.h"
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
};
}