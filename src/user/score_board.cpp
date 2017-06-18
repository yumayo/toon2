﻿#include "score_board.h"
#include "scene_manager.h"
#include "renderer/label.h"
#include "utility.hpp"
#include "action.hpp"
#include "player_manager.h"
using namespace cinder;
namespace user
{
CREATE_CPP( score_board, std::shared_ptr<node> player_manager, cinder::vec2 size )
{
    CREATE( score_board, player_manager, size );
}
bool score_board::init( std::shared_ptr<node> player_manager, cinder::vec2 size )
{
    set_name( "paint_score" );

    _player_manager = player_manager;

    set_anchor_point( vec2( 1, 0 ) );
    set_position( vec2( app::getWindowWidth( ), 0 ) );
    set_content_size( size );
    set_color( ColorA( 1, 1, 1, 0.5F ) );
    set_pivot( vec2( 0, 0 ) );

    auto l = renderer::label::create( get_name( ), "misaki_gothic.ttf", 48 );
    l->set_color( ColorA( 0, 0, 0 ) );
    l->set_anchor_point( vec2( 0.5F, 0 ) );
    l->set_position( vec2( get_content_size( ).x / 2, 6 ) );
    add_child( l );

    auto scores = rect::create( vec2( size.x - 10 * 2, size.y - 42 - 10 ) );
    scores->set_color( ColorA( 1, 1, 1, 0.5F ) );
    scores->set_position( vec2( 10, 42 ) );
    scores->set_anchor_point( vec2( 0 ) );
    scores->set_pivot( vec2( 0 ) );
    scores->set_name( "scores" );
    add_child( scores );

    auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );
    _tcp_connection = std::dynamic_pointer_cast<network::tcp_client>( dont_destroy_node.lock( )->get_child_by_name( "tcp_connection" ) );

    _tcp_connection.lock( )->on_received_named_json.insert( std::make_pair( "ranking", [ this ] ( Json::Value root )
    {
        auto scores = get_child_by_name( "scores" );
        scores->remove_all_children( );
        auto player_mgr = std::dynamic_pointer_cast<user::player_manager>( _player_manager.lock( ) );
        player_mgr->remove_all_crown( );

        run_action( action::call_func::create( [ this, root ]
        {
            auto scores = get_child_by_name( "scores" );
            int i = 1;
            std::vector<int> ids( 3 );
            for ( auto& rank : root["data"] )
            {
                if ( i - 1 < ids.size( ) ) ids[i - 1] = rank["id"].asInt( );

                int rank_number = i;

                auto l = renderer::label::create( utility::format( " %-3d %-3d", rank_number, rank["id"].asInt( ) ), "misaki_gothic.ttf", 32 );
                l->set_color( ColorA( 0, 0, 0 ) );
                l->set_anchor_point( vec2( 0, 0 ) );
                l->set_position( vec2( 0, ( rank_number - 1 ) * 32 ) );
                scores->add_child( l );
                auto s = renderer::label::create( utility::format( "%5d ", rank["score"].asInt( ) ), "misaki_gothic.ttf", 32 );
                s->set_color( ColorA( 0, 0, 0 ) );
                s->set_anchor_point( vec2( 1, 0 ) );
                s->set_position( vec2( scores->get_content_size( ).x, ( rank_number - 1 ) * 32 ) );
                scores->add_child( s );

                i++;
            }

            auto player_mgr = std::dynamic_pointer_cast<user::player_manager>( _player_manager.lock( ) );
            player_mgr->set_all_crown( ids );
        } ) );
    } ) );

    return true;
}
}
