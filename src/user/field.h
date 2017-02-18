#pragma once
#include "../renderer/surface.h"
namespace user
{
class field : public renderer::surface
{
public:
    CREATE_H( field );
    bool init( );
    void update( float delta ) override;
public:
    void feed_collide_update( std::weak_ptr<node> player );
    void player_paint( std::weak_ptr<node> player );
private:
    std::weak_ptr<node> _feed_manager;
};
}