#pragma once
#include "node.h"
#include "network.hpp"
#include "jsoncpp/json.h"
namespace user
{
class feed_manager : public node
{
public:
    CREATE_H( feed_manager, std::weak_ptr<node> player_manager, Json::Value const& feed_root );
    bool init( std::weak_ptr<node> player_manager, Json::Value const& feed_root );
    void update( float delta ) final override;
    void on_feed_captured( Json::Value const& root );
protected:
    std::weak_ptr<node> _player_manager;
    std::weak_ptr<network::tcp_client> _tcp_connection;
};
}