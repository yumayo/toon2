#pragma once
#include "cocoslike.hpp"
#include "jsoncpp/json.h"
#include "network/udp_connection.h"
#include "synchronization_objects.h"
namespace user
{
class search_room : public node
{
    std::weak_ptr<network::udp_connection> _udp_connection;
    std::weak_ptr<network::tcp_client> _tcp_connection;
    Json::Value _root_buffer;
    bool _is_founded = false;
    std::vector<feed_data> _feed_buffer;
    std::vector<bullet_data> _bullet_buffer;
    std::vector<std::vector<ground_data>> _ground_buffer;
public:
    CREATE_H( search_room );
    bool init( );
public:
    std::function<void( Json::Value& root_buffer,
                        std::vector<feed_data> feed_buffet,
                        std::vector<bullet_data>& bullet_buffer,
                        std::vector<std::vector<ground_data>>& ground_buffer )> on_founded;
    std::function<void( )> on_not_found;
};
}