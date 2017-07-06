#include "division_cell_manager.h"
#include "feed.h"
#include "cell_manager.h"
#include <treelike/scene_manager.h>
#include <treelike/user_default.h>
#include <treelike/utility.hpp>
#include <treelike/action.hpp>
#include "se.h"
#include "division_cell.h"
using namespace cinder;
using namespace treelike;
namespace user
{
CREATE_CPP( division_cell_manager, softptr<node> cell_manager, std::vector<Json::Value> const& division_cells_buffer )
{
    CREATE( division_cell_manager, cell_manager, division_cells_buffer );
}
bool division_cell_manager::init( softptr<node> cell_manager, std::vector<Json::Value> const& division_cells_buffer )
{
    set_name( "division_cell_manager" );

    _cell_manager = cell_manager;

    _tcp_connection = scene_manager::get_instans( )->get_dont_destroy_node( )->get_child_by_name( "tcp_connection" )
        .dynamicptr<network::tcp_client>( );

    for ( auto& c : division_cells_buffer )
    {
        auto& data = c["data"];
        int id = data["id"].asInt( );
        cinder::vec2 position = vec2( data["position"][0].asFloat( ), data["position"][1].asFloat( ) );
        cinder::vec2 direction = vec2( data["position"][0].asFloat( ), data["position"][1].asFloat( ) );
        add_child( division_cell::create( c.first, ) );
    }

    _tcp_connection->on_received_named_json.insert( std::make_pair( "feed_captured", [ this ] ( Json::Value root )
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
            _tcp_connection->write( Json::FastWriter( ).write( _captured_feed_data ) );

            _captured_feed_number = 0;
            _captured_feed_data.clear( );
        }
    } ) ) ) );

    set_schedule_update( );

    return true;
}
void division_cell_manager::update( float delta )
{
    for ( auto child : _cell_manager->get_children( ) )
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

