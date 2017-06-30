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
                 int port, std::string const& skin_relative_path )
{
    auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );
    _tcp_connection = std::dynamic_pointer_cast<network::tcp_client>( dont_destroy_node.lock( )->get_child_by_name( "tcp_connection" ) );

    _handle = std::make_shared<network::network_object>( ip_address, port );

    _skin_relative_path = skin_relative_path;

    _radius = user_default::get_instans( )->get_root( )["system"]["min_radius"].asFloat( );
    if ( auto base = renderer::circle::create( _radius ) )
    {
        _base = base;
        add_child( base );

        if ( auto mask = _skin_relative_path.empty( ) ?
             renderer::circle::create( _radius ) : skin::create( _radius, _radius, _skin_relative_path ) )
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
std::string const & cell::get_skin_relative_path( ) const
{
    return _skin_relative_path;
}
float cell::get_radius( )
{
    return _radius;
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
