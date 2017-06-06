#pragma once
#include "player_manager.h"
#include "jsoncpp/json.h"
namespace user
{
class player_manager_host : public player_manager
{
public:
    CREATE_H( player_manager_host, Json::Value& root, std::shared_ptr<network::udp_connection>& connection );
    bool init( Json::Value& root, std::shared_ptr<network::udp_connection>& connection );
    void update( float delta )final override;
};
}