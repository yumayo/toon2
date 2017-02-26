#include "ground.h"
#include "player.h"
#include "player_manager.h"
#include "cinder/Rand.h"
using namespace cinder;
namespace user
{
CREATE_CPP( ground, std::weak_ptr<node> player_manager )
{
    CREATE( ground, player_manager );
}
bool ground::init( std::weak_ptr<node> player_manager )
{
    if ( !renderer::surface::init( vec2( 512 ), ColorA( 0.1F, 0.1F, 0.1F ) ) ) return false;

    set_name( "ground" );

    _player_manager = player_manager;

    set_schedule_update( );

    set_scale( vec2( 4.0F ) );

    // プレイヤーの位置をランダムに決める。
    spawn_player( );

    return true;
}
void ground::update( float delta )
{
    for ( auto child : _player_manager.lock( )->get_children( ) )
    {
        auto pla = std::dynamic_pointer_cast<player>( child );
        if ( !pla ) continue;

        // フィールドに色を塗る。
        float radius = pla->get_radius( ) / get_scale( ).x;
        for ( int y = -radius; y < radius; ++y )
        {
            for ( int x = -radius; x < radius; ++x )
            {
                auto len = length( vec2( x, y ) );
                if ( len < radius )
                {
                    set_pixel( pla->get_position( ) / get_scale( ).x + vec2( x, y ), pla->get_color( ) );
                }
            }
        }

        // フィールド制限。
        auto pos = pla->get_position( );
        auto size = get_content_size( ) * get_scale( );
        pos.x = clamp( pos.x, 0.0F, size.x );
        pos.y = clamp( pos.y, 0.0F, size.y );
        pla->set_position( pos );
    }
}
void ground::spawn_player( )
{
    auto pla_mgr = std::dynamic_pointer_cast<user::player_manager>( _player_manager.lock( ) );
    auto pla = pla_mgr->get_player( );
    Rand rand( app::getElapsedSeconds( ) * 100 );
    auto size = get_content_size( ) * get_scale( );
    pla->set_position( { rand.nextFloat( 0.0F, size.x ), rand.nextFloat( 0.0F, size.y ) } );
}
}

