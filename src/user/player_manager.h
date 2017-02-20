#pragma once
#include "../node.h"
#include "player.h"
namespace user
{
class player_manager : public node
{
public:
    CREATE_H( player_manager );
    bool init( );
    void update( float delta ) override;
protected:
    std::weak_ptr<player> _player;
public:
    std::shared_ptr<player> get_player( );
protected:
    std::weak_ptr<player> _enemy;
};
}