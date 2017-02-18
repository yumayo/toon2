#include "feed.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "../action/action.hpp"
using namespace cinder;
namespace user
{
CREATE_CPP( feed )
{
    CREATE( feed );
}
bool feed::init( )
{
    if ( !renderer::sprite::init( "feed.png" ) ) return false;

    set_color( hsvToRgb( vec3( randFloat( ), randFloat( 0.5, 0.8 ), randFloat( 0.5, 0.8 ) ) ) );
    set_rotation( randFloat( M_PI ) );
    set_scale( vec2( randFloat( 1.1F, 1.3F ) ) );
    _score = ( get_scale( ).x - 1.0F ) * 10;

    using namespace action;
    auto seq = sequence::create( ease<EaseInOutExpo>::create( scale_by::create( 0.2F, vec2( 0.2 ) ) ),
                                 ease<EaseInOutExpo>::create( scale_by::create( 0.2F, vec2( -0.2 ) ) )
    );
    run_action( repeat_forever::create( seq ) );
    run_action( repeat_forever::create( rotate_by::create( 5.0F, M_PI * 2 ) ) );

    return true;
}
float feed::capture( )
{
    remove_from_parent( );
    return _score;
}
}