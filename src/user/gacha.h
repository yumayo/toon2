#pragma once
#include <treelike.hpp>
namespace user
{
class gacha : public treelike::scene
{
public:
    CREATE_H( gacha );
    bool init( );
    bool mouse_began( cinder::app::MouseEvent event ) override;
    bool touch_began( cinder::app::TouchEvent::Touch event ) override;
private:
    softptr<treelike::node> _maschine;
    softptr<treelike::node> _garagara;
    softptr<treelike::node> _back_button;
    softptr<treelike::node> _feed;
    softptr<treelike::renderer::label> _feed_number_label;
    void change_action( std::function<void( )> end_fn );
    std::string get_new_skin_name( );
    bool is_complete( );
    bool _is_animation_end = false;
    std::vector<std::string> _skin_items;
    softptr<treelike::node> _3d;
};
}