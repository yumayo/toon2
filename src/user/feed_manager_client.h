#pragma once
#include "feed_manager.h"
#include "jsoncpp/json.h"
namespace user
{
class feed_manager_client : public feed_manager
{
public:
    CREATE_H( feed_manager_client, std::weak_ptr<node> player_manager );
    bool init( std::weak_ptr<node> player_manager );
    void update( float delta ) final override;
public:
    void create_feed( Json::Value const& root );
};
}