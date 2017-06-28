#include "division_cell_manager.h"
#include "feed.h"
#include "cell_manager.h"
#include "scene_manager.h"
#include "user_default.h"
#include "utility.hpp"
#include "action.hpp"
#include "se.h"
#include "division_cell.h"
using namespace cinder;
namespace user
{
CREATE_CPP( division_cell_manager, std::weak_ptr<node> cell_manager, std::vector<Json::Value> const& division_cells_buffer )
{
    CREATE( division_cell_manager, cell_manager, division_cells_buffer );
}
bool division_cell_manager::init( std::weak_ptr<node> cell_manager, std::vector<Json::Value> const& division_cells_buffer )
{
    set_name( "division_cell_manager" );

    _cell_manager = cell_manager;

    _tcp_connection = std::dynamic_pointer_cast<network::tcp_client>(
        scene_manager::get_instans( )->get_dont_destroy_node( ).lock( )->get_child_by_name( "tcp_connection" ) );

    for ( auto& c : division_cells_buffer )
    {
        auto& data = c["data"];
        int id = data["id"].asInt( );
        cinder::vec2 position = vec2( data["position"][0].asFloat( ), data["position"][1].asFloat( ) );
        cinder::vec2 direction = vec2( data["position"][0].asFloat( ), data["position"][1].asFloat( ) );
        add_child( division_cell::create( c.first, ) );
    }

    _tcp_connection.lock( )->on_received_named_json.insert( std::make_pair( "feed_captured", [ this ] ( Json::Value root )
    {
        for ( auto& obj : root["data"] )
        {
            remove_child_by_tag( obj["erase_tag"].asInt( ) );
            add_child( feed::create( obj["tag"].asInt( ), cinder::vec2( obj["position"][0].asInt( ), obj["position"][1].asInt( ) ) ) );
        }
    } ) );

    // エサは0.2秒ごとに同期を取ります。
    // 短い時間でwriteするとどこかで情報をロスします。
    // tcpでもそんなことあるんやな。
    // 送信エラーとかにもならないし。
    run_action( action::repeat_forever::create( action::sequence::create( action::delay::create( 0.2F ), action::call_func::create( [ this ]
    {
        if ( _captured_feed_number != 0 )
        {
            play_se( "sound/captured.wav" );

            _captured_feed_data["name"] = "feed_captured";
            _tcp_connection.lock( )->write( Json::FastWriter( ).write( _captured_feed_data ) );

            _captured_feed_number = 0;
            _captured_feed_data.clear( );
        }
    } ) ) ) );

    set_schedule_update( );

    return true;
}
void division_cell_manager::update( float delta )
{
    for ( auto child : _cell_manager.lock( )->get_children( ) )
    {
        if ( auto pla = std::dynamic_pointer_cast<cell>( child ) )
        {
            for ( auto& f : _children )
            {
                auto fee = std::dynamic_pointer_cast<feed>( f );
                if ( fee->is_captureing( ) ) continue;

                // 自分の半径の二倍分の距離から吸い取れます。
                if ( distance( fee->get_position( ), pla->get_position( ) )
                     < fee->get_radius( ) * 2 + pla->get_radius( ) )
                {
                    fee->captured( pla );
                }
            }
        }
    }
}
void division_cell_manager::on_feed_captured( int tag )
{
    utility::log( "[%d]on_feed_captured", tag );
    _captured_feed_data["data"][_captured_feed_number++]["tag"] = tag;
}
}

