#pragma once
#include "../node.h"
#include "../renderer/sprite.h"
namespace user
{
class controller : public node
{
public:
    CREATE_H( controller, std::weak_ptr<node> player );
    bool init( std::weak_ptr<node> player );
    bool touch_began( cinder::app::TouchEvent::Touch event ) override;
    void touch_moved( cinder::app::TouchEvent::Touch event ) override;
    void touch_ended( cinder::app::TouchEvent::Touch event ) override;
    void update( float delta ) override;
public:
    cinder::vec2 get_axis( );
private:
    cinder::vec2 tap_start_position;
    cinder::vec2 axis;
    std::weak_ptr<renderer::sprite> _base_node;
    std::weak_ptr<renderer::sprite> _axis_node;
    std::weak_ptr<node> _player;
};
}