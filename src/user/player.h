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
public:
    void on_captured( softptr<treelike::node> other ) override;
    void capture( float score );
    void scale_action( float score );
    void blowout( float power );
    float get_target_radius( );
protected:
    float _target_radius = 0.0F;
    softptr<treelike::node> _cell_manager;
};
}