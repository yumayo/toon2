#pragma once
#include "../node.h"
namespace user
{
class feed_manager : public node
{
public:
    CREATE_H( feed_manager, std::weak_ptr<node> player_manager, std::weak_ptr<node> ground );
    bool init( std::weak_ptr<node> player_manager, std::weak_ptr<node> ground );
    void update( float delta ) override;
private:
    std::weak_ptr<node> _player_manager;
};
}