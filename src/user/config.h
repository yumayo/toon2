#pragma once
#include <treelike.hpp>
#include <cinder/gl/GlslProg.h>
namespace user
{
class config : public treelike::scene
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
    softptr<treelike::node> _slider;
    cinder::vec2 _tap_start_position;
    cinder::vec2 _tap_position;
    cinder::vec2 _tap_prev_position;
    cinder::vec2 _tap_start_slide_object_position;
    cinder::vec2 _object_select_position;
    std::vector<std::string> _skin_names;
    bool _is_switched = true;
    float _skin_width = 200.0F;
private:
    softptr<treelike::node> _bac;
    softptr<treelike::node> _edg;
    void change_action( std::function<void( )> end_fn );
};
}