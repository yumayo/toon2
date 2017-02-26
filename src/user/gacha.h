#pragma once
#include "cocoslike.h"
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
    std::weak_ptr<node> _mas;
    std::weak_ptr<node> _gar;
    std::weak_ptr<node> _bac;
    std::weak_ptr<dot_label> _fla;
    void change_action( std::function<void( )> end_fn );
    std::string get_new_skin_name( );
};
}