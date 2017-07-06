#include "create_dot_obeject.h"
#include "cinder/Surface.h"
using namespace cinder;
using namespace treelike;
namespace user
{
std::shared_ptr<treelike::node> create_dot( std::string const & relative_path, float target_width )
{
    Surface32f csv = loadImage( app::loadAsset( relative_path ) );

    float dot_size = target_width / csv.getWidth( );

    auto logo = node::create( );
    logo->set_content_size( vec2( csv.getSize( ) ) * dot_size );
    logo->set_anchor_point( vec2( 0.5F ) );

    auto len = length( vec2( csv.getSize( ) ) );

    for ( int y = 0; y < csv.getHeight( ); ++y )
    {
        for ( int x = 0; x < csv.getWidth( ); ++x )
        {
            if ( csv.getPixel( ivec2( x, y ) ) == ColorA::black( ) )
            {
                auto dot = renderer::rect::create( { dot_size, dot_size } );
                dot->set_position( vec2( dot_size ) * 0.5F + vec2( x, y ) * dot_size );
                dot->set_scale( vec2( 0.0F ) );
                auto act_scale = action::ease<EaseOutBounce>::create( action::scale_to::create( 0.3, vec2( 1.0F ) ) );
                auto up = action::ease<EaseOutBounce>::create( action::scale_by::create( 0.3F, vec2( -0.2F ) ) );
                auto down = action::ease<EaseOutBounce>::create( action::scale_by::create( 0.3F, vec2( 0.2F ) ) );
                auto roop = action::repeat_forever::create( action::sequence::create( action::delay::create( 2.0F ), up, down ) );
                dot->run_action( action::sequence::create( action::delay::create( 0.3F + float( x + y ) / len ), act_scale, roop ) );
                logo->add_child( dot );
            }
        }
    }

    return logo;
}
std::shared_ptr<button_scale> create_dot_button( std::string const & relative_path, float target_width )
{
    Surface32f csv = loadImage( app::loadAsset( relative_path ) );

    float dot_size = target_width / csv.getWidth( );

    auto logo = button_scale::create( );
    logo->set_content_size( vec2( csv.getSize( ) ) * dot_size );
    logo->set_anchor_point( vec2( 0.5F ) );

    auto len = length( vec2( csv.getSize( ) ) );

    for ( int y = 0; y < csv.getHeight( ); ++y )
    {
        for ( int x = 0; x < csv.getWidth( ); ++x )
        {
            if ( csv.getPixel( ivec2( x, y ) ) == ColorA::black( ) )
            {
                auto dot = renderer::rect::create( { dot_size, dot_size } );
                dot->set_position( vec2( dot_size ) * 0.5F + vec2( x, y ) * dot_size );
                dot->set_scale( vec2( 0.0F ) );
                auto act_scale = action::ease<EaseOutBounce>::create( action::scale_to::create( 0.3, vec2( 1.0F ) ) );
                auto up = action::ease<EaseOutBounce>::create( action::scale_by::create( 0.3F, vec2( -0.2F ) ) );
                auto down = action::ease<EaseOutBounce>::create( action::scale_by::create( 0.3F, vec2( 0.2F ) ) );
                auto roop = action::repeat_forever::create( action::sequence::create( action::delay::create( 2.0F ), up, down ) );
                dot->run_action( action::sequence::create( action::delay::create( 0.3F + float( x + y ) / len ), act_scale, roop ) );
                logo->add_child( dot );
            }
        }
    }

    return logo;
}
}