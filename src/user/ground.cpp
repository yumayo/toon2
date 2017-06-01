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
    if ( !renderer::surface::init( vec2( 4096 ), ColorA( 0.1F, 0.1F, 0.1F ) ) ) return false;

    gl::Texture::Format format;
    format.setMinFilter( GL_NEAREST );
    format.setMagFilter( GL_NEAREST );
    _texture = gl::Texture::create( _surface, format );

    set_name( "ground" );

    _player_manager = player_manager;

    set_schedule_update( );

    set_scale( vec2( 1.0F ) );

    spawn_player( );

    return true;
}
void ground::update( float delta )
{
    // プレイヤーマネージャからプレイヤーを抽出し、グラウンドに色を塗る。
    if ( auto& player_mgr = std::dynamic_pointer_cast<player_manager>( _player_manager.lock( ) ) )
    {
        player_paint_ground( player_mgr->get_player( ) );

        for ( auto& client : player_mgr->get_clients( ) )
        {
            player_paint_ground( client );
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
void ground::spawn_player( )
{
    auto pla_mgr = _player_manager.lock( );
    auto pla = pla_mgr->get_child_by_name( "own" );
    Rand rand( app::getElapsedSeconds( ) * 100 );
    auto size = get_content_size( ) * get_scale( );
    pla->set_position( { rand.nextFloat( 0.0F, size.x ), rand.nextFloat( 0.0F, size.y ) } );
}
void ground::player_paint_ground( std::weak_ptr<node> player_node )
{
    if ( !player_node.lock( ) )return;
    auto player = std::dynamic_pointer_cast<user::player>( player_node.lock( ) );
    float radius = player->get_radius( ) / get_scale( ).x;
    paint_fill_circle( player->get_position( ) / get_scale( ).x, radius, player->get_color( ) );
}
}

