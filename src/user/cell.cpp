#include "cell.h"
#include "action.hpp"
#include "utility/string_utility.h"
#include "skin.h"
#include "cinder/gl/Texture.h"
#include "scene_manager.h"
using namespace cinder;
namespace user
{
bool cell::init( std::string const& ip_address,
                   int port, std::string const& relative_path_skin )
{
    auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );
    _tcp_connection = std::dynamic_pointer_cast<network::tcp_client>( dont_destroy_node.lock( )->get_child_by_name( "tcp_connection" ) );

    _handle = std::make_shared<network::network_object>( ip_address, port );

    _setup_radius = 20.0F;

    _radius = _setup_radius;
    if ( auto base = renderer::circle::create( _radius ) )
    {
        _base = base;
        add_child( base );

        if ( auto mask = relative_path_skin.empty( ) ?
             renderer::circle::create( _radius ) : skin::create( _radius, relative_path_skin ) )
        {
            _mask = mask;
            mask->set_scale( vec2( 0.9F ) );
            base->add_child( mask );
        }
    }

    return true;
}
cell::~cell( )
{
}
float cell::get_radius( )
{
    return _radius;
}
float cell::get_startup_radius( )
{
    return _setup_radius;
}
void cell::set_radius( float value )
{
    _radius = value;
    _base.lock( )->set_radius( get_radius( ) );
    _mask.lock( )->set_radius( get_radius( ) );

    if ( auto crown = _base.lock( )->get_child_by_name( "crown" ) )
    {
        crown->set_scale( vec2( _radius / ( crown->get_content_size( ).x / 2 ) ) );
    }
}
void user::cell::set_color( cinder::ColorA value )
{
    node::set_color( value );
    _mask.lock( )->set_color( value );
}
void cell::remove_crown( )
{
    if ( auto crown = _base.lock( )->get_child_by_name( "crown" ) )
    {
        crown->remove_from_parent( );
    }
}
void cell::set_crown( std::weak_ptr<node> crown )
{
    crown.lock( )->set_name( "crown" );
    _base.lock( )->add_child( crown.lock( ) );

    if ( auto crown = _base.lock( )->get_child_by_name( "crown" ) )
    {
        crown->set_scale( vec2( _radius / ( crown->get_content_size( ).x / 2 ) ) );
    }
}
bool cell::is_crowner( )
{
    return _base.lock( )->get_child_by_name( "crown" ) != nullptr;
}
network::network_handle user::cell::get_handle( )
{
    return _handle;
}
}
