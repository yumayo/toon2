#pragma once
#include "renderer/surface.h"
#include "player.h"
#include "jsoncpp/json.h"
namespace user
{
class ground : public renderer::surface
{
public:
    CREATE_H( ground, std::weak_ptr<node> player_manager, Json::Value const& root );
    bool init( std::weak_ptr<node> player_manager, Json::Value const& root );
    void update( float delta ) override;
public:
    // Controller�݂̂������܂��B
    // ���g�̃v���C���[�݂̂̓����蔻��v�Z�ł��ˁB
    void collide( std::weak_ptr<node> player );
    void close_player( cinder::ColorA const& color );
private:
    void player_paint_ground( std::weak_ptr<player> player );
private:
    std::weak_ptr<node> _player_manager;
};
}