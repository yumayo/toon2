#pragma once
#include "renderer/surface.h"
#include "player.h"
#include "jsoncpp/json.h"
namespace user
{
class ground : public renderer::surface
{
public:
    CREATE_H( ground, std::weak_ptr<node> player_manager, Json::Value const& root, std::vector<std::vector<unsigned char>>& ground_buffer );
    bool init( std::weak_ptr<node> player_manager, Json::Value const& root, std::vector<std::vector<unsigned char>>& ground_buffer );
    void update( float delta ) override;
public:
    // Controllerのみが扱います。
    // 自身のプレイヤーのみの当たり判定計算ですね。
    void collide( std::weak_ptr<node> player );
    void close_player( cinder::ColorA const& color );
private:
    void cell_paint_ground( std::weak_ptr<cell> cell );
private:
    std::weak_ptr<node> _player_manager;
};
}