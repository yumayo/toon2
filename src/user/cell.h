#pragma once
#include "captured_object.h"
#include <treelike/renderer/circle.h>
#include <treelike/network.hpp>
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
    void set_color( cinder::ColorA const& value ) override;
    void remove_crown( );
    void set_crown( hardptr<treelike::node> crown );
    bool is_crowner( );
    treelike::network::network_handle get_handle( );
protected:
    softptr<treelike::renderer::circle> _base;
    softptr<treelike::renderer::circle> _mask;
    float _radius = 0.0F;
    hardptr<treelike::network::network_handle> _handle;
    softptr<treelike::network::tcp_client> _tcp_connection;
    std::string _skin_relative_path;
};
}