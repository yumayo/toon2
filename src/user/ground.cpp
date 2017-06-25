#include "ground.h"
#include "player.h"
#include "player_manager.h"
#include "user_default.h"
using namespace cinder;
namespace user
{
CREATE_CPP( ground, std::weak_ptr<node> player_manager, Json::Value const& root, std::vector<std::vector<unsigned char>>& ground_buffer )
{
    CREATE( ground, player_manager, root, ground_buffer );
}
bool ground::init( std::weak_ptr<node> player_manager, Json::Value const& root, std::vector<std::vector<unsigned char>>& ground_buffer )
{
    if ( !renderer::surface::init( vec2( root["data"]["ground_size"].asInt( ) ), ColorA( 0.1F, 0.1F, 0.1F ) ) ) return false;

    std::map<int, ColorA> color_map;
    color_map.insert( std::make_pair( 0, ColorA( 0.1F, 0.1F, 0.1F ) ) ); // ID 0 は黒色にします。
    color_map.insert( std::make_pair( root["data"]["id"].asInt( ), ColorA( root["data"]["color"][0].asFloat( ),
                                                                           root["data"]["color"][1].asFloat( ),
                                                                           root["data"]["color"][2].asFloat( ) ) ) );
    for ( auto& j : root["data"]["clients"] )
    {
        color_map.insert( std::make_pair( j["id"].asInt( ), ColorA( j["color"][0].asFloat( ),
                                                                    j["color"][1].asFloat( ),
                                                                    j["color"][2].asFloat( ) ) ) );
    }

    for ( int y = 0; y < _surface.getHeight( ); ++y )
    {
        for ( int x = 0; x < _surface.getWidth( ); ++x )
        {
            auto id = ground_buffer[x][y];
            auto color = color_map[id];
            _surface.setPixel( ivec2( x, y ), color );
        }
    }

    gl::Texture::Format format;
    format.setMinFilter( GL_NEAREST );
    format.setMagFilter( GL_NEAREST );
    _texture = gl::Texture::create( _surface, format );

    set_name( "ground" );

    _player_manager = player_manager;

    set_schedule_update( );

    set_scale( vec2( root["data"]["ground_scale"].asInt( ) ) );

    return true;
}
void ground::update( float delta )
{
    // プレイヤーマネージャからプレイヤーを抽出し、グラウンドに色を塗る。
    if ( auto& player_mgr = std::dynamic_pointer_cast<player_manager>( _player_manager.lock( ) ) )
    {
        cell_paint_ground( player_mgr->get_player( ) );

        for ( auto& enemy : player_mgr->get_enemys( ) )
        {
            cell_paint_ground( enemy );
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
void ground::close_player( cinder::ColorA const & color )
{
    for ( int y = 0; y < _surface.getHeight( ); ++y )
    {
        for ( int x = 0; x < _surface.getWidth( ); ++x )
        {
            if ( ColorA8u( color ) == _surface.getPixel( ivec2( x, y ) ) )
            {
                _surface.setPixel( ivec2( x, y ), ColorA( 0.1F, 0.1F, 0.1F ) );
            }
        }
    }
    _texture->update( _surface );
}
void ground::cell_paint_ground( std::weak_ptr<cell> cell )
{
    if ( !cell.lock( ) ) return;
    float radius = cell.lock( )->get_radius( ) / get_scale( ).x;
    paint_fill_circle( cell.lock( )->get_position( ) / get_scale( ).x, radius, cell.lock( )->get_color( ) );
}
}

