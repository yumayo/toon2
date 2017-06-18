#pragma once
#include "captured_object.h"
#include "renderer/circle.h"
#include "cinder/gl/Texture.h"
#include "network.hpp"
namespace user
{
class player : public captured_object
{
public:
    CREATE_H( player, std::string const& ip_address,
              int port, std::string const& relative_path_skin = "" );
    bool init( std::string const& ip_address,
               int port, std::string const& relative_path_skin );
    virtual ~player( );
public:
    float get_radius( );
    float get_startup_radius( );
    void set_radius( float value );
    void on_captured( std::weak_ptr<node> other ) override;
    void capture( float score );
    void set_color( cinder::ColorA value ) override;
    void remove_crown( );
    void set_crown( std::weak_ptr<node> crown );
    bool is_crowner( );
    network::network_handle get_handle( );
protected:
    std::weak_ptr<renderer::circle> _base;
    std::weak_ptr<renderer::circle> _mask;
    float _radius = 0.0F;
    float _target_radius = 0.0F;
    float _setup_radius = 20.0F;
    std::shared_ptr<network::network_object> _handle;
    std::weak_ptr<network::tcp_client> _tcp_connection;

};
}