#pragma once
#include "cell.h"
namespace user
{
class player : public cell
{
public:
    CREATE_H( player, std::weak_ptr<node> cell_manager, std::string const& ip_address,
              int port, std::string const& relative_path_skin = "" );
    bool init( std::weak_ptr<node> cell_manager, std::string const& ip_address,
               int port, std::string const& relative_path_skin );
    ~player( );
    void key_down( cinder::app::KeyEvent e );
public:
    void on_captured( std::weak_ptr<node> other ) override;
    void capture( float score );
    void scale_action( float score );
    void blowout( );
    bool parse_bullet_data( Json::Value* root, cinder::vec2 direction );
protected:
    float _target_radius = 0.0F;
    std::weak_ptr<node> _cell_manager;
};
}