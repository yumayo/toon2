#pragma once
#include "node.h"
#include "player.h"
#include "network.hpp"
namespace user
{
class player_manager : public node
{
public:
    CREATE_H( player_manager, Json::Value& root );
    bool init( Json::Value& root );
    virtual void update( float delta )override;
public:
    std::list<std::shared_ptr<node>>& get_clients( );
    std::weak_ptr<player>& get_player( );
protected:
    std::weak_ptr<node> _clients;
    std::weak_ptr<player> _player;
    std::weak_ptr<network::udp_connection> _udp_connection;
    std::weak_ptr<network::tcp_client> _tcp_connection;
    void create_client( Json::Value const& root_client );
};
}