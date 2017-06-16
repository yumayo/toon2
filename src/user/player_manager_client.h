#pragma once
#include "player_manager.h"
#include "jsoncpp/json.h"
namespace user
{
class player_manager_client : public player_manager
{
public:
    CREATE_H( player_manager_client, Json::Value& root );
    bool init( Json::Value& root );
    void update( float delta )final override;
};
}