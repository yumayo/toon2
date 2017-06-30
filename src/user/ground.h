#pragma once
#include "renderer/surface_cubic.h"
#include "player.h"
#include "jsoncpp/json.h"
#include "bullet.h"
#include "synchronization_objects.h"
namespace user
{
class ground : public renderer::surface_cubic
{
public:
    CREATE_H( ground, std::weak_ptr<node> cell_manager, std::weak_ptr<node> bullet_manager, Json::Value const& root, std::vector<std::vector<ground_data>>& ground_buffer );
    bool init( std::weak_ptr<node> cell_manager, std::weak_ptr<node> bullet_manager, Json::Value const& root, std::vector<std::vector<ground_data>>& ground_buffer );
    void update( float delta ) override;
public:
    // Controllerのみが扱います。
    // 自身のプレイヤーのみの当たり判定計算ですね。
    void collide( std::weak_ptr<node> player );
    void close_player( cinder::ColorA const& color );
private:
    void paint_ground_cell( std::weak_ptr<cell> cell );
    void paint_ground_bullet( std::weak_ptr<bullet> bullet );
private:
    std::weak_ptr<node> _bullet_manager;
    std::weak_ptr<node> _cell_manager;
};
}