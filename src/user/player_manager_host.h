#pragma once
#include "player_manager.h"
#include "../network/udp_server.h"
namespace user
{
class player_manager_host : public player_manager
{
public:
    CREATE_H( player_manager_host );
    bool init( );
    void update( float delta ) override;
protected:
    std::weak_ptr<network::udp_server> _server;
};
}