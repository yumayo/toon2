#pragma once
#include "renderer/surface.h"
#include "player.h"
#include "jsoncpp/json.h"
namespace user
{
class ground : public renderer::surface
{
public:
    CREATE_H( ground, std::weak_ptr<node> player_manager, int const& ground_size );
    bool init( std::weak_ptr<node> player_manager, int const& ground_size );
    void update( float delta ) override;
public:
    // Controllerのみが扱います。
    // 自身のプレイヤーのみの当たり判定計算ですね。
    void collide( std::weak_ptr<node> player );
private:
    void player_paint_ground( std::weak_ptr<player> player );
private:
    std::weak_ptr<node> _player_manager;
};
}