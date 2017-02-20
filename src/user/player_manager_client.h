#pragma once
#include "player_manager.h"
#include "../network/udp_client.h"
namespace user
{
class player_manager_client : public player_manager
{
public:
    CREATE_H( player_manager_client );
    bool init( );
    void update( float delta ) override;
protected:
    std::weak_ptr<network::udp_client> _client;
};
}