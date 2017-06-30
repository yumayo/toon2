#include "bullet_manager.h"
#include "cell_manager.h"
#include "scene_manager.h"
#include "bullet.h"
#include "utility.hpp"
#include "action.hpp"
using namespace cinder;
namespace user
{
CREATE_CPP( bullet_manager, std::weak_ptr<node> cell_manager, Json::Value bullet_buffer )
{
    CREATE( bullet_manager, cell_manager, bullet_buffer );
}
bool bullet_manager::init( std::weak_ptr<node> cell_manager, Json::Value bullet_buffer )
{
    set_schedule_update( );

    set_name( "bullet_manager" );

    _cell_manager = cell_manager;

    auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );
    _tcp_connection = std::dynamic_pointer_cast<network::tcp_client>( dont_destroy_node.lock( )->get_child_by_name( "tcp_connection" ) );

    _tcp_connection.lock( )->on_received_named_json.insert( std::make_pair( "create_bullet", [ this ] ( Json::Value root )
    {
        for ( auto& data : root["data"] )
        {
            auto bullet_id = data["bullet_id"].asInt( );
            auto user_id = data["user_id"].asInt( );
            auto pos = vec2( data["position"][0].asFloat( ), data["position"][1].asFloat( ) );
            auto direction = vec2( data["direction"][0].asFloat( ), data["direction"][1].asFloat( ) );
            auto time_offset = data["time_offset"].asFloat( );
            if ( auto cell = std::dynamic_pointer_cast<user::cell>( _cell_manager.lock( )->get_child_by_tag( user_id ) ) )
            {
                auto color = cell->get_color( );
                auto skin_relative_path = cell->get_skin_relative_path( );

                auto bullet = std::make_shared<user::bullet>( );
                if ( bullet && bullet->init( bullet_id, time_offset, pos, direction, color, skin_relative_path ) );
                else bullet.reset( );

                auto n = get_child_by_tag( user_id );
                if ( !n )
                {
                    n = add_child( node::create( ) );
                    n->set_tag( user_id );
                }
                n->add_child( bullet );
            }
            else
            {
                utility::log( "【bullet_manager】対象のプレイヤー[%d]が見つかりませんでした。", user_id );
            }
        }
    } ) );

    run_action( action::repeat_forever::create( action::sequence::create( action::delay::create( 0.2F ), action::call_func::create( [ this ]
    {
        if ( _number_of_created_bullet != 0 )
        {
            //play_se( "sound/captured.wav" );

            _created_bullet_data["name"] = "create_bullet";
            _tcp_connection.lock( )->write( Json::FastWriter( ).write( _created_bullet_data ) );

            _number_of_created_bullet = 0;
            _created_bullet_data.clear( );
        }
        _time_offset = 0.0F;
    } ) ) ) );

    _tcp_connection.lock( )->on_received_named_json.insert( std::make_pair( "erase_bullet", [ this ] ( Json::Value root )
    {
        remove_child_by_tag( root["data"]["id"].asInt( ) );
    } ) );

    auto cell_mgr = std::dynamic_pointer_cast<user::cell_manager>( _cell_manager.lock( ) );
    for ( auto& folder_root : bullet_buffer["data"] )
    {
        for ( auto& bullet_root : folder_root )
        {
            auto pos = vec2( bullet_root["position"][0].asFloat( ), bullet_root["position"][1].asFloat( ) );
            auto direction = vec2( bullet_root["direction"][0].asFloat( ), bullet_root["direction"][1].asFloat( ) );
            auto user_id = bullet_root["user_id"].asInt( );
            auto bullet_id = bullet_root["bullet_id"].asInt( );
            auto time_offset = bullet_root["time_offset"].asInt( );

            if ( auto cell = std::dynamic_pointer_cast<user::cell>( cell_mgr->get_child_by_tag( user_id ) ) )
            {
                auto color = cell->get_color( );
                auto skin_relative_path = cell->get_skin_relative_path( );

                auto bullet = std::make_shared<user::bullet>( );
                if ( bullet && bullet->init( bullet_id, time_offset, pos, direction, color, skin_relative_path ) );
                else bullet.reset( );

                auto n = get_child_by_tag( user_id );
                if ( !n )
                {
                    n = add_child( node::create( ) );
                    n->set_tag( user_id );
                }
                n->add_child( bullet );
            }
        }
    }

    return true;
}
void bullet_manager::update( float delta )
{
    auto cell_manager = std::dynamic_pointer_cast<user::cell_manager>( _cell_manager.lock( ) );
    auto player = cell_manager->get_player( );
    for ( auto const& c : get_children( ) )
    {
        if ( c->get_color( ) == player.lock( )->get_color( ) ) continue;
        if ( auto const& bullet = std::dynamic_pointer_cast<user::bullet>( c ) )
        {
            if ( bullet->is_hit( ) ) continue;
            if ( distance( player.lock( )->get_position( ), bullet->get_position( ) )
                 < player.lock( )->get_radius( ) + bullet->get_radius( ) )
            {
                Json::Value root;
                root["name"] = "blowout";
                root["data"]["id"] = bullet->get_tag( );
                player.lock( )->blowout( );
                _tcp_connection.lock( )->write( Json::FastWriter( ).write( root ) );
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
    _created_bullet_data["data"][_number_of_created_bullet]["time_offset"] = -_time_offset; // 時間ぶん引きます。
    _number_of_created_bullet++;
}
}

