#pragma once
#include <treelike/node.h>
#include <treelike/renderer/circle.h>
namespace user
{
class analog_axis : public treelike::node
{
public:
    CREATE_H( analog_axis );
    bool init( );
    cinder::vec2 get_normalized_axis( );
    cinder::vec2 get_last_normalized_axis( );
private:
    bool mouse_began( cinder::app::MouseEvent event ) override;
    void mouse_moved( cinder::app::MouseEvent event ) override;
    void mouse_ended( cinder::app::MouseEvent event ) override;
    bool touch_began( cinder::app::TouchEvent::Touch event ) override;
    void touch_moved( cinder::app::TouchEvent::Touch event ) override;
    void touch_ended( cinder::app::TouchEvent::Touch event ) override;
private:
    void began( cinder::vec2 pos );
    void moved( cinder::vec2 pos );
    void ended( cinder::vec2 pos );
private:
    cinder::vec2 _last_normaized_axis;
    cinder::vec2 _tap_start_position;
    cinder::vec2 _axis;
    softptr<treelike::renderer::circle> _base_node;
    softptr<treelike::renderer::circle> _axis_node;
};
}
