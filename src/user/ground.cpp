#include "ground.h"
#include "player.h"
#include "player_manager.h"
#include "user_default.h"
using namespace cinder;
namespace user
{
CREATE_CPP( ground, std::weak_ptr<node> player_manager, int const& ground_size )
{
    CREATE( ground, player_manager, ground_size );
}
bool ground::init( std::weak_ptr<node> player_manager, int const& ground_size )
{
    if ( !renderer::surface::init( vec2( ground_size ), ColorA( 0.1F, 0.1F, 0.1F ) ) ) return false;

    gl::Texture::Format format;
    format.setMinFilter( GL_NEAREST );
    format.setMagFilter( GL_NEAREST );
    _texture = gl::Texture::create( _surface, format );

    set_name( "ground" );

    _player_manager = player_manager;

    set_schedule_update( );

    set_scale( vec2( 1.0F ) );

    return true;
}
void ground::update( float delta )
{
    // プレイヤーマネージャからプレイヤーを抽出し、グラウンドに色を塗る。
    if ( auto& player_mgr = std::dynamic_pointer_cast<player_manager>( _player_manager.lock( ) ) )
    {
        player_paint_ground( player_mgr->get_player( ) );

        for ( auto& enemy : player_mgr->get_enemys( ) )
        {
            player_paint_ground( enemy );
        }
    }
}
void ground::collide( std::weak_ptr<node> player )
{
    auto pos = player.lock( )->get_position( );
    auto size = get_content_size( ) * get_scale( );
    pos.x = clamp( pos.x, 0.0F, size.x );
    pos.y = clamp( pos.y, 0.0F, size.y );
    player.lock( )->set_position( pos );
}
void ground::player_paint_ground( std::weak_ptr<player> player )
{
    if ( !player.lock( ) ) return;
    float radius = player.lock( )->get_radius( ) / get_scale( ).x;
    paint_fill_circle( player.lock( )->get_position( ) / get_scale( ).x, radius, player.lock( )->get_color( ) );
}
}

