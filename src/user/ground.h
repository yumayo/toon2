#pragma once
#include "../renderer/surface.h"
namespace user
{
class ground : public renderer::surface
{
public:
    CREATE_H( ground, std::weak_ptr<node> player_manager );
    bool init( std::weak_ptr<node> player_manager );
    void update( float delta ) override;
private:
    void spawn_player( );
private:
    std::weak_ptr<node> _player_manager;
};
}