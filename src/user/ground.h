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
    // Controller�݂̂������܂��B
    // ���g�̃v���C���[�݂̂̓����蔻��v�Z�ł��ˁB
    void collide( std::weak_ptr<node> player );
private:
    // �Ƃ肠���������_���Ȉʒu�ɐ������Ă��܂��B
    void spawn_player( );
    void player_paint_ground( std::weak_ptr<node> player );
private:
    std::weak_ptr<node> _player_manager;
};
}