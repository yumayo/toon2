#pragma once
#include "cocoslike.hpp"
#include "dot_label.h"
namespace user
{
class gacha : public scene
{
public:
    CREATE_H( gacha );
    bool init( );
    bool mouse_began( cinder::app::MouseEvent event ) override;
    bool touch_began( cinder::app::TouchEvent::Touch event ) override;
private:
    std::weak_ptr<node> _maschine;
    std::weak_ptr<node> _garagara;
    std::weak_ptr<node> _back_button;
    std::weak_ptr<renderer::label> _feed_number_label;
    void change_action( std::function<void( )> end_fn );
    std::string get_new_skin_name( );
    bool is_complete( );
    bool _is_animation_end = false;
    std::vector<std::string> _skin_items;
};
}