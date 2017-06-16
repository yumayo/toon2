#pragma once
#include "feed_manager.h"
#include "cinder/Rand.h"
namespace user
{
class feed_manager_host : public feed_manager
{
public:
    CREATE_H( feed_manager_host, std::weak_ptr<node> player_manager, std::weak_ptr<node> ground );
    bool init( std::weak_ptr<node> player_manager, std::weak_ptr<node> ground );
    void update( float delta ) final override;
public:
    void create_feed( );
private:
    std::weak_ptr<node> _ground;
    int _now_tag = 0;
    std::shared_ptr<cinder::Rand> _random;
};
}