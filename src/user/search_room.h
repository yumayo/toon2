#pragma once
#include "cocoslike.hpp"
#include "jsoncpp/json.h"
#include "network/udp_connection.h"
namespace user
{
class search_room : public node
{
    std::weak_ptr<network::udp_connection> _udp_connection;
    std::weak_ptr<network::tcp_client> _tcp_connection;
public:
    CREATE_H( search_room );
    bool init( );
public:
    std::function<void( Json::Value& root )> on_founded;
    std::function<void( )> on_not_found;
};
}