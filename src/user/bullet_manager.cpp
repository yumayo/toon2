#include "bullet_manager.h"
#include "cell_manager.h"
#include "bullet.h"
#include "bullet_straight.h"
#include <treelike/scene_manager.h>
#include <treelike/utility.hpp>
#include <treelike/action.hpp>
using namespace cinder;
using namespace treelike;
namespace user
{
CREATE_CPP( bullet_manager, softptr<node> cell_manager, Json::Value bullet_buffer )
{
    CREATE( bullet_manager, cell_manager, bullet_buffer );
}
bool bullet_manager::init( softptr<node> cell_manager, Json::Value bullet_buffer )
{
    set_schedule_update( );

    set_name( "bullet_manager" );

    _cell_manager = cell_manager;

    auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );
    _tcp_connection = dont_destroy_node->get_child_by_name( "tcp_connection" ).dynamicptr<network::tcp_client>( );

    _tcp_connection->on( "create_bullet", [ this ] ( Json::Value root )
    {
        for ( auto& data : root["data"] )
        {
            auto bullet_id = data["bullet_id"].asInt( );
            auto user_id = data["user_id"].asInt( );
            auto start_position = vec2( data["start_position"][0].asFloat( ), data["start_position"][1].asFloat( ) );
            auto end_position = vec2( data["end_position"][0].asFloat( ), data["end_position"][1].asFloat( ) );
            if ( auto cell = _cell_manager->get_child_by_tag( user_id ).dynamicptr<user::cell>( ) )
            {
                auto color = cell->get_color( );
                auto skin_relative_path = cell->get_skin_relative_path( );

                auto folder = get_child_by_tag( user_id );
                if ( !folder )
                {
                    folder = add_child( node::create( ) );
                    folder->set_tag( user_id );
                }
                folder->add_child( bullet_straight::create( bullet_id, start_position, end_position, color, skin_relative_path ) );
            }
            else
            {
                utility::log( "【bullet_manager】対象のプレイヤー[%d]が見つかりませんでした。", user_id );
            }
        }
    } );

    run_action( action::repeat_forever::create( action::sequence::create( action::delay::create( 0.2F ), action::call_func::create( [ this ]
    {
        if ( _number_of_created_bullet != 0 )
        {
            _created_bullet_data["name"] = "create_bullet";
            _tcp_connection->write( Json::FastWriter( ).write( _created_bullet_data ) );

            _number_of_created_bullet = 0;
            _created_bullet_data.clear( );
        }
        _time_offset = 0.0F;
    } ) ) ) );

    _tcp_connection->on( "erase_bullet", [ this ] ( Json::Value root )
    {
        for ( auto& folder : get_children( ) )
        {
            for ( auto& bullet_node : folder->get_children( ) )
            {
                bullet_node->remove_child_by_tag( root["data"]["id"].asInt( ) );
            }
        }
    } );

    auto cell_mgr = _cell_manager.dynamicptr<user::cell_manager>( );
    for ( auto& folder_root : bullet_buffer["data"] )
    {
        for ( auto& bullet_root : folder_root )
        {
            auto pos = vec2( bullet_root["position"][0].asFloat( ), bullet_root["position"][1].asFloat( ) );
            auto direction = vec2( bullet_root["direction"][0].asFloat( ), bullet_root["direction"][1].asFloat( ) );
            auto user_id = bullet_root["user_id"].asInt( );
            auto bullet_id = bullet_root["bullet_id"].asInt( );

            if ( auto cell = cell_mgr->get_child_by_tag( user_id ).dynamicptr<user::cell>( ) )
            {
                auto color = cell->get_color( );
                auto skin_relative_path = cell->get_skin_relative_path( );

                auto folder = get_child_by_tag( user_id );
                if ( !folder )
                {
                    folder = add_child( node::create( ) );
                    folder->set_tag( user_id );
                }
                folder->add_child( bullet_straight::create( bullet_id, pos, direction, color, skin_relative_path ) );
            }
        }
    }

    return true;
}
void bullet_manager::update( float delta )
{
    auto cell_manager = _cell_manager.dynamicptr<user::cell_manager>( );
    auto player = cell_manager->get_player( );
    for ( auto const& folder : get_children( ) )
    {
        for ( auto const& bullet_node : folder->get_children( ) )
        {
            if ( folder->get_tag( ) == player->get_tag( ) ) continue;
            if ( auto const& bullet = std::dynamic_pointer_cast<user::bullet>( bullet_node ) )
            {
                if ( bullet->is_hit( ) ) continue;
                if ( distance( player->get_position( ), bullet->get_position( ) )
                     < player->get_radius( ) + bullet->get_radius( ) )
                {
                    bullet->hit( );
                    Json::Value root;
                    root["name"] = "blowout";
                    root["data"]["id"] = bullet->get_tag( );
                    player->blowout( );
                    _tcp_connection->write( Json::FastWriter( ).write( root ) );
                }
            }
        }
    }
    _time_offset += delta;
}
void bullet_manager::close_player( cinder::ColorA const& color )
{
    for ( auto const& c : get_children( ) )
    {
        if ( c->get_color( ) == color )
        {
            c->remove_from_parent( );
        }
    }
}
void bullet_manager::create_bullet( Json::Value const & data )
{
    _created_bullet_data["data"][_number_of_created_bullet] = data;
    _number_of_created_bullet++;

    auto bullet_id = data["bullet_id"].asInt( );
    auto user_id = data["user_id"].asInt( );
    auto start_position = vec2( data["start_position"][0].asFloat( ), data["start_position"][1].asFloat( ) );
    auto end_position = vec2( data["end_position"][0].asFloat( ), data["end_position"][1].asFloat( ) );
    if ( auto cell = _cell_manager->get_child_by_tag( user_id ).dynamicptr<user::cell>( ) )
    {
        auto color = cell->get_color( );
        auto skin_relative_path = cell->get_skin_relative_path( );

        auto folder = get_child_by_tag( user_id );
        if ( !folder )
        {
            folder = add_child( node::create( ) );
            folder->set_tag( user_id );
        }
        folder->add_child( bullet_straight::create( bullet_id, start_position, end_position, color, skin_relative_path ) );
    }
}
}

