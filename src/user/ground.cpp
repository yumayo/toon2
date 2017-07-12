#include "ground.h"
#include "player.h"
#include "cell_manager.h"
#include "bullet_manager.h"
#include "bullet.h"
#include <treelike/user_default.h>
using namespace cinder;
using namespace treelike;
namespace user
{
CREATE_CPP( ground, softptr<node> cell_manager, softptr<node> bullet_manager, Json::Value const& root, std::vector<std::vector<ground_data>>& ground_buffer )
{
    CREATE( ground, cell_manager, bullet_manager, root, ground_buffer );
}
bool ground::init( softptr<node> cell_manager, softptr<node> bullet_manager, Json::Value const& root, std::vector<std::vector<ground_data>>& ground_buffer )
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
    if ( _is_inserted )
    {
        for ( int i = _min_insert_iterator; i < _past_paint_datas.size( ); ++i )
        {
            paint_fill_circle( _past_paint_datas[i].position, _past_paint_datas[i].radius, _past_paint_datas[i].color );
        }
        _is_inserted = false;
        _min_insert_iterator = 0;
    }

    if ( auto& cell_manager = _cell_manager.dynamicptr<user::cell_manager>( ) )
    {
        for ( auto const& c : cell_manager->get_children( ) )
        {
            paint_ground_cell( std::dynamic_pointer_cast<cell>( c ) );
        }
    }

    if ( auto& bullet_manager = _bullet_manager.dynamicptr<user::bullet_manager>( ) )
    {
        for ( auto const& folder : bullet_manager->get_children( ) )
        {
            for ( auto const& bullet_node : folder->get_children( ) )
            {
                paint_ground_bullet( bullet_node );
            }
        }
    }

    // 古いデータは切り捨てます。
    int i = 0;
    for ( ; i < _past_paint_datas.size( ); ++i )
    {
        if ( _past_paint_datas[i].time > app::getElapsedSeconds( ) - 0.5F )
        {
            break;
        }
    }
    _past_paint_datas.erase( _past_paint_datas.begin( ), _past_paint_datas.begin( ) + i );
}
void ground::collide( softptr<node> player )
{
    auto pos = player->get_position( );
    auto size = get_content_size( ) * get_scale( );
    pos.x = clamp( pos.x, 0.0F, size.x );
    pos.y = clamp( pos.y, 0.0F, size.y );
    player->set_position( pos );
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
void ground::insert( float time, cinder::vec2 position, float radius, cinder::ColorA color )
{
    _is_inserted = true;
    int i = std::max( 0, (int)_past_paint_datas.size( ) - 1 );
    for ( ; i >= 0; --i )
    {
        if ( _past_paint_datas[i].time < time ) break;
    }
    _min_insert_iterator = i;
    _past_paint_datas.insert( _past_paint_datas.begin( ) + i, paint_data( { time, position, radius, color } ) );
}
void ground::paint_ground_cell( softptr<treelike::node> cell_node )
{
    if ( !cell_node ) return;
    auto cell = cell_node.dynamicptr<user::cell>( );
    float radius = cell->get_radius( ) / get_scale( ).x;
    paint_ground( cell->get_position( ) / get_scale( ).x, radius, cell->get_color( ) );
}
void ground::paint_ground_bullet( softptr<treelike::node> bullet_node )
{
    if ( !bullet_node ) return;
    auto bullet = bullet_node.dynamicptr<user::bullet>( );
    float radius = bullet->get_radius( ) / get_scale( ).x;
    paint_ground( bullet->get_position( ) / get_scale( ).x, radius, bullet->get_color( ) );
}
void ground::paint_ground( cinder::vec2 position, float radius, cinder::ColorA color )
{
    _past_paint_datas.push_back( { (float)app::getElapsedSeconds( ), position, radius, color } );
    paint_fill_circle( position, radius, color );
}
}

