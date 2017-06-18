#include "score_board.h"
#include "scene_manager.h"
#include "renderer/label.h"
#include "utility.hpp"
#include "action.hpp"
using namespace cinder;
namespace user
{
CREATE_CPP( score_board, cinder::vec2 size )
{
    CREATE( score_board, size );
}
bool score_board::init( cinder::vec2 size )
{
    set_name( "score_board" );

    set_anchor_point( vec2( 1, 0 ) );
    set_position( vec2( app::getWindowWidth( ), 0 ) );
    set_content_size( size );
    set_color( ColorA( 1, 1, 1, 0.5F ) );
    set_pivot( vec2( 0, 0 ) );

    auto l = renderer::label::create( get_name( ), "misaki_gothic.ttf", 48 );
    l->set_color( ColorA( 0, 0, 0 ) );
    l->set_anchor_point( vec2( 0.5F, 0 ) );
    l->set_position( vec2( get_content_size( ).x / 2, 0 ) );
    add_child( l );

    auto scores = rect::create( vec2( size.x - 10 * 2, size.y - 48 - 10 ) );
    scores->set_color( ColorA( 1, 1, 1, 0.5F ) );
    scores->set_position( vec2( 10, 48 ) );
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

        run_action( action::call_func::create( [ this, root ]
        {
            auto scores = get_child_by_name( "scores" );
            int i = 1;
            for ( auto& rank : root["data"] )
            {
                scores->add_child( create_score( i++, rank["id"].asInt( ), rank["score"].asInt( ) ) );
            }
        } ) );
    } ) );

    return true;
}
std::shared_ptr<node> score_board::create_score( int rank, int id, int score )
{
    auto l = renderer::label::create( utility::format( "%-3d %3d[%5d]", rank, id, score ), "misaki_gothic.ttf", 32 );
    l->set_color( ColorA( 0, 0, 0 ) );
    l->set_anchor_point( vec2( 0, 0 ) );
    l->set_position( vec2( 0, ( rank - 1 ) * 32 ) );
    return l;
}
}

