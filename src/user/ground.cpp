#include "ground.h"
#include "player.h"
#include "cell_manager.h"
#include "bullet_manager.h"
#include "user_default.h"
using namespace cinder;
namespace user
{
CREATE_CPP( ground, std::weak_ptr<node> cell_manager, std::weak_ptr<node> bullet_manager, Json::Value const& root, std::vector<std::vector<ground_data>>& ground_buffer )
{
    CREATE( ground, cell_manager, bullet_manager, root, ground_buffer );
}
bool ground::init( std::weak_ptr<node> cell_manager, std::weak_ptr<node> bullet_manager, Json::Value const& root, std::vector<std::vector<ground_data>>& ground_buffer )
{
    if ( !renderer::surface_cubic::init( vec2( root["data"]["ground_size"].asInt( ) ), ColorA( 0.1F, 0.1F, 0.1F ) ) ) return false;

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
            auto id = ground_buffer[x][y].id;
            auto color = color_map[id];
            _surface.setPixel( ivec2( x, y ), color );
        }
    }

    _texture->update( _surface );

    set_name( "ground" );

    _bullet_manager = bullet_manager;
    _cell_manager = cell_manager;

    set_schedule_update( );

    set_scale( vec2( root["data"]["ground_scale"].asInt( ) ) );

    return true;
}
void ground::update( float delta )
{
    if ( auto& cell_manager = std::dynamic_pointer_cast<user::cell_manager>( _cell_manager.lock( ) ) )
    {
        for ( auto const& c : cell_manager->get_children( ) )
        {
            paint_ground_cell( std::dynamic_pointer_cast<cell>( c ) );
        }
    }

    if ( auto& bullet_manager = std::dynamic_pointer_cast<user::bullet_manager>( _bullet_manager.lock( ) ) )
    {
        for ( auto const& folder : bullet_manager->get_children( ) )
        {
            for ( auto const& bullet_node : folder->get_children( ) )
            {
                paint_ground_bullet( std::dynamic_pointer_cast<bullet>( bullet_node ) );
            }
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
void ground::paint_ground_cell( std::weak_ptr<cell> cell )
{
    if ( !cell.lock( ) ) return;
    float radius = cell.lock( )->get_radius( ) / get_scale( ).x;
    paint_fill_circle( cell.lock( )->get_position( ) / get_scale( ).x, radius, cell.lock( )->get_color( ) );
}
void ground::paint_ground_bullet( std::weak_ptr<bullet> bullet )
{
    if ( !bullet.lock( ) ) return;
    float radius = bullet.lock( )->get_radius( ) / get_scale( ).x;
    paint_fill_circle( bullet.lock( )->get_position( ) / get_scale( ).x, radius, bullet.lock( )->get_color( ) );
}
}

