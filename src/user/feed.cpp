#include "feed.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "feed_manager.h"
#include "../action/action.hpp"
#include "player.h"
using namespace cinder;
namespace user
{
CREATE_CPP( feed )
{
    CREATE( feed );
}
bool feed::init( )
{
    set_name( "feed" );

    if ( auto circle = renderer::circle::create( 12, 5 ) )
    {
        _circle = circle;
        circle->set_color( hsvToRgb( vec3( randFloat( ), randFloat( 0.5, 0.8 ), randFloat( 0.5, 0.8 ) ) ) );
        circle->set_scale( vec2( 0.0F ) );

        float target_scale = randFloat( 1.1F, 1.3F );
        _score = ( target_scale - 1.0F ) * 10;

        using namespace action;
        auto seq = sequence::create( ease<EaseInOutExpo>::create( scale_by::create( 0.2F, vec2( 0.2 ) ) ),
                                     ease<EaseInOutExpo>::create( scale_by::create( 0.2F, vec2( -0.2 ) ) )
        );
        circle->run_action( sequence::create( ease<EaseOutSine>::create( scale_to::create( 0.2F, vec2( target_scale ) ) ),
                                              repeat_forever::create( seq ) ) );

        circle->run_action( repeat_forever::create( rotate_by::create( 5.0F, randBool( ) ? M_PI * 2 : -M_PI * 2 ) ) );

        add_child( circle );
    }

    return true;
}
float feed::get_radius( )
{
    return _circle.lock( )->get_radius( );
}
void feed::on_captured( std::weak_ptr<node> other )
{
    auto pla = std::dynamic_pointer_cast<user::player>( other.lock( ) );
    pla->capture( _score );
    auto mgr = std::dynamic_pointer_cast<user::feed_manager>( get_parent( ) );
    pla->packet.add_captured_feed( std::make_pair( get_tag( ), mgr->create_feed( ) ) );
}
}