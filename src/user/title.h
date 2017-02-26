#pragma once
#include "cocoslike.h"
#include "button_scale.h"
namespace user
{
class title : public scene
{
public:
    CREATE_H( title );
    bool init( );
private:
    std::weak_ptr<node> _tit;
    std::weak_ptr<node> _sta;
    std::weak_ptr<node> _gac;
    std::weak_ptr<node> _con;
    void change_action( std::function<void( )> end_fn );
};
}