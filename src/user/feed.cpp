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
    if ( !renderer::circle::init( 12, 5 ) ) return false;

    set_name( "feed" );
    set_color( hsvToRgb( vec3( randFloat( ), randFloat( 0.5, 0.8 ), randFloat( 0.5, 0.8 ) ) ) );
    set_scale( vec2( 0.0F ) );
    float target_scale = randFloat( 1.1F, 1.3F );
    _score = ( target_scale - 1.0F ) * 10;

    using namespace action;
    auto seq = sequence::create( ease<EaseInOutExpo>::create( scale_by::create( 0.2F, vec2( 0.2 ) ) ),
                                 ease<EaseInOutExpo>::create( scale_by::create( 0.2F, vec2( -0.2 ) ) )
    );
    run_action( sequence::create( ease<EaseOutSine>::create( scale_to::create( 0.2F, vec2( target_scale ) ) ),
                                  repeat_forever::create( seq ) ) );

    run_action( repeat_forever::create( rotate_by::create( 5.0F, randBool( ) ? M_PI * 2 : -M_PI * 2 ) ) );

    return true;
}

void feed::captured( std::weak_ptr<node> player )
{
    _captureing = true;

    auto cpt = [ this, player ]
    {
        auto pla = std::dynamic_pointer_cast<user::player>( player.lock( ) );
        pla->capture( _score );
        auto mgr = std::dynamic_pointer_cast<user::feed_manager>( get_parent( ) );
        mgr->create_feed( );
    };

    run_action( action::sequence::create( action::move_to_target::create( 0.3F, player.lock( ) ),
                                          action::call_func::create( std::move( cpt ) ),
                                          action::remove_self::create( ) ) );
}
bool feed::captureing( )
{
    return _captureing;
}
}