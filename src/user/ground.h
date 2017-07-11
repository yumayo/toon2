#pragma once
#include <treelike/renderer/surface_cubic.h>
#include "player.h"
#include "jsoncpp/json.h"
#include "bullet.h"
#include "synchronization_objects.h"
namespace user
{
class ground : public treelike::renderer::surface_cubic
{
public:
    CREATE_H( ground, softptr<treelike::node> cell_manager, softptr<treelike::node> bullet_manager, Json::Value const& root, std::vector<std::vector<ground_data>>& ground_buffer );
    bool init( softptr<treelike::node> cell_manager, softptr<treelike::node> bullet_manager, Json::Value const& root, std::vector<std::vector<ground_data>>& ground_buffer );
    void update( float delta ) override;
public:
    // Controllerのみが扱います。
    // 自身のプレイヤーのみの当たり判定計算ですね。
    void collide( softptr<treelike::node> player );
    void close_player( cinder::ColorA const& color );
public:
    void paint_ground_cell( softptr<cell> cell );
    void paint_ground_bullet( softptr<bullet> bullet );
private:
    softptr<treelike::node> _bullet_manager;
    softptr<treelike::node> _cell_manager;
};
}