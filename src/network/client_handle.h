#pragma once
#include <string>
#include "../forward.h"
#include "sol.hpp"
namespace network
{
class client_handle
{
public:
    client_handle( std::string const& name, std::string const& ip_address, int const& port );
public:
    friend bool operator==( std::string const&, client_handle const& );
    bool operator==( std::string const& name ) const;
    bool operator==( client_handle const& other ) const;
public: // 値は tcp_server のみが書き換えられます。
    std::string const& _name;
    std::string const& _ip_address;
    int const& _port;
private: // lua専用関数群。
    std::string get_name( )const;
    std::string get_ip_address( )const;
    int get_port( )const;
public:
    LUA_SETUP_H( client_handle );
};
bool operator==( std::string const& name, client_handle const& other );
}
