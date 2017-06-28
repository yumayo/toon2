#include "division_cell.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "feed_manager.h"
#include "action.hpp"
#include "player.h"
#include "utility.hpp"
using namespace cinder;
namespace user
{
CREATE_CPP( division_cell, int tag, cinder::vec2 position, cinder::vec2 normalized_direction, float radius, cinder::Color const& color, std::string const& skin_relative_path )
{
    CREATE( division_cell, tag, position, normalized_direction, radius, color, skin_relative_path );
}
bool division_cell::init( int tag, cinder::vec2 position, cinder::vec2 normalized_direction, float radius, cinder::Color const& color, std::string const& skin_relative_path )
{
    utility::log( "create_division_cell: [%d]", tag );

    set_name( "division_cell" );

    set_tag( tag );

    set_position( position );

    if ( auto circle = renderer::circle::create( radius, radius ) )
    {
        _circle = circle;
        circle->set_color( color );

        _score = radius;

        using namespace action;
        circle->run_action( ease<cinder::EaseOutCubic>::create( move_to::create( 2.0F, position + normalized_direction * radius ) ) );

        add_child( circle );
    }

    return true;
}
float division_cell::get_radius( )
{
    return _circle.lock( )->get_radius( );
}
void division_cell::on_captured( std::weak_ptr<node> other )
{
    if ( auto pla = std::dynamic_pointer_cast<user::player>( other.lock( ) ) )
    {
        pla->capture( _score );

        if ( auto feed_mgr = std::dynamic_pointer_cast<feed_manager>( get_parent( ) ) )
        {
            feed_mgr->on_feed_captured( get_tag( ) );
        }
    }
}
}