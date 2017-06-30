#pragma once
#include "captured_object.h"
#include "renderer/circle.h"
#include "network.hpp"
namespace user
{
class cell : public captured_object
{
public:
    bool init( std::string const& ip_address, int port, std::string const& relative_path_skin );
    virtual ~cell( );
public:
    std::string const& get_skin_relative_path( ) const;
    float get_radius( );
    void set_radius( float value );
    void set_color( cinder::ColorA value ) override;
    void remove_crown( );
    void set_crown( std::weak_ptr<node> crown );
    bool is_crowner( );
    network::network_handle get_handle( );
protected:
    std::weak_ptr<renderer::circle> _base;
    std::weak_ptr<renderer::circle> _mask;
    float _radius = 0.0F;
    std::shared_ptr<network::network_object> _handle;
    std::weak_ptr<network::tcp_client> _tcp_connection;
    std::string _skin_relative_path;
};
}