#pragma once
#include "cocoslike.hpp"
namespace user
{
class config : public scene
{
public:
    CREATE_H( config );
    bool init( );
    void update( float delta ) override;
    bool mouse_began( cinder::app::MouseEvent event ) override;
    void mouse_moved( cinder::app::MouseEvent event ) override;
    void mouse_ended( cinder::app::MouseEvent event ) override;
    bool touch_began( cinder::app::TouchEvent::Touch event )override;
    void touch_moved( cinder::app::TouchEvent::Touch event )override;
    void touch_ended( cinder::app::TouchEvent::Touch event )override;
private:
    std::weak_ptr<node> _slide;
    cinder::vec2 _tap_start_position;
    cinder::vec2 _tap_position;
    cinder::vec2 _tap_prev_position;
    cinder::vec2 _tap_start_slide_object_position;
    cinder::vec2 _object_select_position;
    std::vector<std::string> _skin_names;
    float _slide_power = 0.0F;
    float _friction = 30.0F;
private:
    std::weak_ptr<node> _bac;
    std::weak_ptr<node> _edg;
    void change_action( std::function<void( )> end_fn );
};
}