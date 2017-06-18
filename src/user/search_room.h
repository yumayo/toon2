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
    Json::Value _root_buffer;
    bool _is_founded = false;
    std::map<int, cinder::ivec2> _feeds_buffer;
    std::vector<std::vector<unsigned char>> _ground_buffer;
public:
    CREATE_H( search_room );
    bool init( );
public:
    std::function<void( Json::Value& root_buffer, 
                        std::map<int, cinder::ivec2>& feeds_buffer,
                        std::vector<std::vector<unsigned char>>& ground_buffer )> on_founded;
    std::function<void( )> on_not_found;
};
}