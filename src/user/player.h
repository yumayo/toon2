#pragma once
#include "cell.h"
namespace user
{
class player : public cell
{
public:
    CREATE_H( player, std::string const& ip_address,
              int port, std::string const& relative_path_skin = "" );
    bool init( std::string const& ip_address,
               int port, std::string const& relative_path_skin );
    ~player( );
public:
    void on_captured( std::weak_ptr<node> other ) override;
    void capture( float score );
    void scale_action( float score );
    void blowout( );
protected:
    float _target_radius = 0.0F;
};
}