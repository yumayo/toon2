#include "cell.h"
#include <treelike/action.hpp>
#include "skin.h"
#include "cinder/gl/Texture.h"
#include <treelike/scene_manager.h>
using namespace cinder;
using namespace treelike;
namespace user
{
bool cell::init( std::string const& ip_address,
                 int port, std::string const& skin_relative_path )
{
    auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );
    _tcp_connection = dont_destroy_node->get_child_by_name( "tcp_connection" ).dynamicptr<network::tcp_client>( );

    _handle = make_hard<network::network_handle>( ip_address, port );

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
            mask->set_name( "cell_mask" );
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
    _base->set_radius( get_radius( ) );
    _mask->set_radius( get_radius( ) );

    if ( auto crown = _base->get_child_by_name( "crown" ) )
    {
        crown->set_scale( vec2( _radius / ( crown->get_content_size( ).x / 2 ) ) );
    }
}
void user::cell::set_color( cinder::ColorA const& value )
{
    node::set_color( value );
    _mask->set_color( value );
}
void cell::remove_crown( )
{
    _base->remove_child_by_name( "crown" );
}
void cell::set_crown( hardptr<node> crown )
{
    crown->set_scale( vec2( 2.0F ) );
    crown->set_name( "crown" );
    _base->add_child( crown );

    if ( auto crown = _base->get_child_by_name( "crown" ) )
    {
        crown->set_scale( vec2( _radius / ( crown->get_content_size( ).x / 2 ) ) );
    }
}
bool cell::is_crowner( )
{
    return _base->get_child_by_name( "crown" ) != nullptr;
}
network::network_handle user::cell::get_handle( )
{
    return *_handle;
}
}
