#pragma once
#include <treelike.hpp>
#include "button_scale.h"
namespace user
{
class title : public treelike::scene
{
public:
    CREATE_H( title );
    bool init( );
private:
    softptr<treelike::node> _title_logo;
    softptr<treelike::node> _start_button;
    softptr<treelike::node> _gacha_button;
    softptr<treelike::node> _config_button;
    void change_action( std::function<void( )> end_fn );
};
}