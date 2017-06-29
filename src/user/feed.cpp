#include "feed.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "feed_manager.h"
#include "action.hpp"
#include "player.h"
#include "utility.hpp"
#include "cube.h"
using namespace cinder;
namespace user
{
CREATE_CPP( feed, int tag, cinder::vec2 position )
{
    CREATE( feed, tag, position );
}
bool feed::init( int tag, cinder::vec2 position )
{
    utility::log( "create_feed: [%d]", tag );

    set_name( "feed" );

    set_tag( tag );

    set_position( position );

    _radius = 12.0F;

    if ( auto object = cube::create( vec3( _radius ) ) )
    {
        object->set_color( hsvToRgb( vec3( randFloat( ), randFloat( 0.5, 0.8 ), randFloat( 0.5, 0.8 ) ) ) );
        object->set_scale( vec2( 0.0F ) );

        float target_scale = randFloat( 1.1F, 1.3F );
        _score = ( target_scale - 1.0F ) * 10;

        using namespace action;
        auto seq = sequence::create( ease<EaseInOutExpo>::create( scale_by::create( 0.2F, vec2( 0.2 ) ) ),
                                     ease<EaseInOutExpo>::create( scale_by::create( 0.2F, vec2( -0.2 ) ) )
        );
        object->run_action( sequence::create( ease<EaseOutSine>::create( scale_to::create( 0.2F, vec2( target_scale ) ) ),
                                              repeat_forever::create( seq ) ) );

        object->run_action( repeat_forever::create( rotate_by::create( 5.0F, randBool( ) ? M_PI * 2 : -M_PI * 2 ) ) );

        add_child( object );
    }

    return true;
}
float feed::get_radius( )
{
    return _radius;
}
void feed::on_captured( std::weak_ptr<node> other )
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