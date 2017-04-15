#pragma once
#include "node.h"
#include "player.h"
#include "network.hpp"
namespace user
{
class player_manager : public node
{
    void create_client( Json::Value const& root_client );
public:
    bool init( Json::Value& root, std::shared_ptr<network::udp_connection>& connection );
    void update( float delta ) override;
protected:
    std::weak_ptr<node> _clients;
    std::weak_ptr<player> _player;
    std::weak_ptr<network::udp_connection> _udp;
};
}