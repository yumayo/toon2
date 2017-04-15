#pragma once
#include "node.h"
#include "player.h"
#include "network.hpp"
namespace user
{
class player_manager : public node
{
public:
    bool init( Json::Value& root );
    void update( float delta ) override;
protected:
    std::weak_ptr<node> _clients;
    std::weak_ptr<player> _player;
    std::weak_ptr<network::udp_connection> _udp;
};
}