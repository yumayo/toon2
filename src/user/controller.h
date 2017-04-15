#pragma once
#include "node.h"
#include "renderer/circle.h"
#include "player.h"
#include "ground.h"
namespace user
{
class controller : public node
{
public:
    CREATE_H( controller, std::weak_ptr<player> player, std::weak_ptr<ground> ground );
    bool init( std::weak_ptr<player> player, std::weak_ptr<ground> ground );
    bool mouse_began( cinder::app::MouseEvent event ) override;
    void mouse_moved( cinder::app::MouseEvent event ) override;
    void mouse_ended( cinder::app::MouseEvent event ) override;
    bool touch_began( cinder::app::TouchEvent::Touch event ) override;
    void touch_moved( cinder::app::TouchEvent::Touch event ) override;
    void touch_ended( cinder::app::TouchEvent::Touch event ) override;
    void update( float delta ) override;
public:
    // ê≥ãKâªÇ≥ÇÍÇΩé≤Çï‘ÇµÇ‹Ç∑ÅB
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
    std::weak_ptr<player> _player;
    std::weak_ptr<ground> _ground;
};
}