#include "bullet_straight.h"
#include "skin.h"
#include <treelike/utility.hpp>
#include <treelike/action.hpp>
#include "cell.h"
using namespace cinder;
using namespace treelike;
namespace user
{
CREATE_CPP( bullet_straight, int bullet_id, float time, cinder::vec2 start_position, cinder::vec2 end_position, cinder::Color color, std::string const& skin_relative_path )
{
    CREATE( bullet_straight, bullet_id, time, start_position, end_position, color, skin_relative_path );
}
bool bullet_straight::init( int bullet_id, float time, cinder::vec2 start_position, cinder::vec2 end_position, cinder::Color color, std::string const& skin_relative_path )
{
    bullet::init( bullet_id, 20.0F, color );

    set_schedule_update( );

    set_position( start_position );

    _base = add_child( user::spike::create( get_radius( ), get_radius( ), 10.0F ) );
    _mask = _base->add_child( skin_relative_path.empty( ) ?
                              renderer::circle::create( get_radius( ) ) :
                              skin::create( get_radius( ), get_radius( ), skin_relative_path ) );
    if ( skin_relative_path.empty( ) )
    {
        _mask->set_color( color );
    }
    _mask->set_scale( vec2( 0.9F ) );

    using namespace action;
    run_action( sequence::create( move_to::create( time, end_position ), remove_self::create( ) ) );

    return true;
}
}