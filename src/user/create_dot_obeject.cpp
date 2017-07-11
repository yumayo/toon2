#include "create_dot_obeject.h"
#include <treelike/renderer.hpp>
#include <treelike/action.hpp>
#include "cinder/Surface.h"
#include <cinder/gl/gl.h>
using namespace cinder;
using namespace treelike;
using namespace treelike::action;
namespace user
{
CREATE_CPP( dot_object, std::string const & relative_path, float target_width )
{
    CREATE( dot_object, relative_path, target_width );
}
bool dot_object::init( std::string const & relative_path, float target_width )
{
    Surface32f csv = loadImage( app::loadAsset( relative_path ) );

    float dot_size = target_width / csv.getWidth( );

    set_content_size( vec2( csv.getSize( ) ) * dot_size );
    set_anchor_point( vec2( 0.5F ) );
    set_axis( vec3( 1, 0, 0 ) );
    set_rotation( -M_PI / 8.0F );

    auto len = length( vec2( csv.getSize( ) ) );

    for ( int y = 0; y < csv.getHeight( ); ++y )
    {
        for ( int x = 0; x < csv.getWidth( ); ++x )
        {
            if ( csv.getPixel( ivec2( x, y ) ) == ColorA::black( ) )
            {
                auto dot = renderer::cube::create( vec3( dot_size ) );
                dot->set_position( vec2( dot_size ) * 0.5F + vec2( x, y ) * dot_size );
                dot->set_scale( vec2( 0.0F ) );
                auto act_scale = ease<EaseOutBounce>::create( scale_to::create( 0.3, vec2( 1.0F ) ) );
                auto up = ease<EaseOutBounce>::create( scale_by::create( 0.3F, vec2( -0.2F ) ) );
                auto down = ease<EaseOutBounce>::create( scale_by::create( 0.3F, vec2( 0.2F ) ) );
                auto roop = repeat_forever::create( sequence::create( delay::create( 2.0F ), up, down ) );
                dot->run_action( sequence::create( delay::create( 0.3F + float( x + y ) / len ), act_scale, roop ) );
                add_child( dot );
            }
        }
    }

    return true;
}
CREATE_CPP( dot_button, std::string const & relative_path, float target_width )
{
    CREATE( dot_button, relative_path, target_width );
}
bool dot_button::init( std::string const & relative_path, float target_width )
{
    rect_button_scale_action::init( );

    Surface32f csv = loadImage( app::loadAsset( relative_path ) );

    float dot_size = target_width / csv.getWidth( );

    set_content_size( vec2( csv.getSize( ) ) * dot_size );
    set_anchor_point( vec2( 0.5F ) );
    set_axis( vec3( 1, 0, 0 ) );
    set_rotation( -M_PI / 8.0F );

    auto len = length( vec2( csv.getSize( ) ) );

    for ( int y = 0; y < csv.getHeight( ); ++y )
    {
        for ( int x = 0; x < csv.getWidth( ); ++x )
        {
            if ( csv.getPixel( ivec2( x, y ) ) == ColorA::black( ) )
            {
                auto dot = renderer::cube::create( vec3( dot_size ) );
                dot->set_position( vec2( dot_size ) * 0.5F + vec2( x, y ) * dot_size );
                dot->set_scale( vec2( 0.0F ) );
                auto act_scale = ease<EaseOutBounce>::create( scale_to::create( 0.3, vec2( 1.0F ) ) );
                auto up = ease<EaseOutBounce>::create( scale_by::create( 0.3F, vec2( -0.2F ) ) );
                auto down = ease<EaseOutBounce>::create( scale_by::create( 0.3F, vec2( 0.2F ) ) );
                auto roop = repeat_forever::create( sequence::create( delay::create( 2.0F ), up, down ) );
                dot->run_action( sequence::create( delay::create( 0.3F + float( x + y ) / len ), act_scale, roop ) );
                add_child( dot );
            }
        }
    }

    return true;
}
}