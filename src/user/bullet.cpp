#include "bullet.h"
#include "skin.h"
#include <treelike/utility.hpp>
#include <treelike/action.hpp>
#include "cell.h"
using namespace cinder;
using namespace treelike;
namespace user
{
bool bullet::init( int bullet_id, float time_offset, cinder::vec2 position, cinder::vec2 direction, cinder::Color color, std::string const& skin_relative_path )
{
    utility::log( "bullet[%d]", bullet_id );

    set_schedule_update( );

    set_tag( bullet_id );
    set_position( position );
    set_color( color );

    _base = add_child( user::spike::create( _radius, _radius, 10.0F ) );
    _mask = _base->add_child( skin_relative_path.empty( ) ?
                                      renderer::circle::create( _radius ) :
                                      skin::create( _radius, _radius, skin_relative_path ) );
    if ( skin_relative_path.empty( ) )
    {
        _mask->set_color( color );
    }
    _mask->set_scale( vec2( 0.9F ) );

    _direction = direction;

    using namespace action;
    run_action( sequence::create( delay::create( 2.0F - time_offset ), remove_self::create( ) ) );

    return true;
}
void bullet::update( float delta )
{
    _position += _direction * 400.0F * delta;
}
float bullet::get_radius( )
{
    return _radius;
}
bool bullet::is_hit( )
{
    return _is_hit;
}
void bullet::hit( )
{
    _is_hit = true;
}
}