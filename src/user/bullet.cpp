#include "bullet.h"
#include "skin.h"
#include "utility.hpp"
#include "action.hpp"
#include "cell.h"
using namespace cinder;
namespace user
{
CREATE_CPP( bullet, int tag, cinder::vec2 position, cinder::vec2 direction, std::weak_ptr<node> node_cell )
{
    CREATE( bullet, tag, position, direction, node_cell );
}
bool bullet::init( int tag, cinder::vec2 position, cinder::vec2 direction, std::weak_ptr<node> node_cell )
{
    utility::log( "bullet[%d]", tag );

    set_schedule_update( );

    set_tag( tag );
    set_position( position );
    set_color( node_cell.lock( )->get_color( ) );

    auto cell = std::dynamic_pointer_cast<user::cell> ( node_cell.lock( ) );

    _base = add_child( user::spike::create( _radius, _radius, 10.0F ) );
    _mask = _base.lock( )->add_child( cell->get_skin_relative_path( ).empty( ) ?
                                      renderer::circle::create( _radius ) :
                                      skin::create( _radius, _radius, cell->get_skin_relative_path( ) ) );
    if ( cell->get_skin_relative_path( ).empty( ) )
    {
        _mask.lock( )->set_color( node_cell.lock( )->get_color( ) );
    }
    _mask.lock( )->set_scale( vec2( 0.9F ) );

    _direction = direction;

    using namespace action;
    run_action( sequence::create( delay::create( 2.0F ), remove_self::create( ) ) );

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