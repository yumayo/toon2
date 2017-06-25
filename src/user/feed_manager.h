#pragma once
#include "node.h"
#include "network.hpp"
#include "jsoncpp/json.h"
namespace user
{
class feed_manager : public node
{
public:
    CREATE_H( feed_manager, std::weak_ptr<node> cell_manager, std::map<int, cinder::ivec2>& feeds_buffer );
    bool init( std::weak_ptr<node> cell_manager, std::map<int, cinder::ivec2>& feeds_buffer );
    void update( float delta ) final override;
    void on_feed_captured( int tag );
protected:
    int _captured_feed_number = 0;
    Json::Value _captured_feed_data;
    std::weak_ptr<node> _cell_manager;
    std::weak_ptr<network::tcp_client> _tcp_connection;
};
}