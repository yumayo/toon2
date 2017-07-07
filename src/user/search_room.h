#pragma once
#include <treelike.hpp>
#include "jsoncpp/json.h"
#include "synchronization_objects.h"
namespace user
{
class search_room : public treelike::node
{
    softptr<treelike::network::udp_connection> _udp_connection;
    softptr<treelike::network::tcp_client> _tcp_connection;
    Json::Value _root_buffer;
    bool _is_founded = false;
    bool _is_failed = false;
    std::vector<feed_data> _feed_buffer;
    Json::Value _bullet_buffer;
    std::vector<std::vector<ground_data>> _ground_buffer;
public:
    CREATE_H( search_room );
    bool init( );
    void update( float delta ) override;
public:
    std::function<void( Json::Value& root_buffer,
                        std::vector<feed_data> feed_buffet,
                        Json::Value& bullet_buffer,
                        std::vector<std::vector<ground_data>>& ground_buffer )> on_founded;
    std::function<void( )> on_not_found;
};
}