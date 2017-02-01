#pragma once
#include "../node.h"
namespace network
{
class tcp_server : public node
{
    struct _member;
    std::shared_ptr<_member> _m;
public:
    struct connect_data
    {
        connect_data( std::string const& ip_address, int port );
        bool operator==( connect_data const& other ) const;
        bool operator<( connect_data const& other ) const;
        std::string ip_address;
        int port;
    };
    CREATE_H( tcp_server, std::string const& port, int num_of_client );
    ~tcp_server( );
    bool init( std::string const& port, int num_of_client );
    std::vector<std::string> get_ip_addresses( );
};
}