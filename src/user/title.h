#pragma once
#include "cocoslike.hpp"
#include "button_scale.h"
namespace user
{
class title : public scene
{
public:
    CREATE_H( title );
    bool init( );
private:
    std::weak_ptr<node> _title_logo;
    std::weak_ptr<node> _start_button;
    std::weak_ptr<node> _gacha_button;
    std::weak_ptr<node> _config_button;
    void change_action( std::function<void( )> end_fn );
};
}