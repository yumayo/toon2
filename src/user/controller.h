#pragma once
#include "../node.h"
#include "../renderer/circle.h"
namespace user
{
class controller : public node
{
public:
    CREATE_H( controller, std::weak_ptr<node> player );
    bool init( std::weak_ptr<node> player );
    bool mouse_began( cinder::app::MouseEvent event ) override;
    void mouse_moved( cinder::app::MouseEvent event ) override;
    void mouse_ended( cinder::app::MouseEvent event ) override;
    bool touch_began( cinder::app::TouchEvent::Touch event ) override;
    void touch_moved( cinder::app::TouchEvent::Touch event ) override;
    void touch_ended( cinder::app::TouchEvent::Touch event ) override;
    void update( float delta ) override;
public:
    cinder::vec2 get_axis( );
private:
    void began( cinder::vec2 pos );
    void moved( cinder::vec2 pos );
    void ended( cinder::vec2 pos );
private:
    cinder::vec2 _tap_start_position;
    cinder::vec2 _axis;
    std::weak_ptr<renderer::circle> _base_node;
    std::weak_ptr<renderer::circle> _axis_node;
    std::weak_ptr<node> _player;
};
}