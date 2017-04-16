#pragma once
#include "renderer/surface.h"
namespace user
{
class ground : public renderer::surface
{
public:
    CREATE_H( ground, std::weak_ptr<node> player_manager );
    bool init( std::weak_ptr<node> player_manager );
    void update( float delta ) override;
public:
    // Controllerのみが扱います。
    // 自身のプレイヤーのみの当たり判定計算ですね。
    void collide( std::weak_ptr<node> player );
private:
    // とりあえずランダムな位置に生成しています。
    void spawn_player( );
    void player_paint_ground( std::weak_ptr<node> player );
private:
    std::weak_ptr<node> _player_manager;
};
}