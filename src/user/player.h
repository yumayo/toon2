#pragma once
#include "cell.h"
namespace user
{
class player : public cell
{
public:
    CREATE_H( player, softptr<treelike::node> cell_manager, std::string const& ip_address,
              int port, std::string const& relative_path_skin = "" );
    bool init( softptr<treelike::node> cell_manager, std::string const& ip_address,
               int port, std::string const& relative_path_skin );
    ~player( );
    void key_down( cinder::app::KeyEvent e );
public:
    void on_captured( softptr<treelike::node> other ) override;
    void capture( float score );
    void scale_action( float score );
    void blowout( );
    bool parse_bullet_data( Json::Value* root, cinder::vec2 direction );
protected:
    float _target_radius = 0.0F;
    softptr<treelike::node> _cell_manager;
};
}