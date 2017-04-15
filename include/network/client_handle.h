#pragma once
#include <string>
#include "forward.h"
namespace network
{
class client_handle
{
public:
    client_handle( std::string const& ip_address, std::string const& port );
public:
    bool operator==( client_handle const& other ) const;
public: // �l�� tcp_server �݂̂������������܂��B
    std::string const& ip_address;
    std::string const& port;
private: // lua��p�֐��Q�B
    std::string get_ip_address( )const;
    std::string get_port( )const;
};
}
