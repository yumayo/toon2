﻿#include "feed_manager.h"
#include "ground.h"
#include "feed.h"
#include "player.h"
#include "../action/action.hpp"
#include "cinder/Rand.h"
#include "player_manager.h"
using namespace cinder;
namespace user
{
CREATE_CPP( feed_manager, std::weak_ptr<node> player_manager, std::weak_ptr<node> ground )
{
    CREATE( feed_manager, player_manager, ground );
}
bool feed_manager::init( std::weak_ptr<node> player_manager, std::weak_ptr<node> ground )
{
    set_name( "feed_manager" );

    _player_manager = player_manager;
    _ground = ground;

    set_schedule_update( );

    run_action( action::repeat_times::create( action::call_func::create( [ this ] { create_feed( ); } ),
                                              25 * _ground.lock( )->get_scale( ).x ) );

    return true;
}
void feed_manager::update( float delta )
{
    // 餌を取れるのはプレイヤーだけです。
    auto pla_mgr = std::dynamic_pointer_cast<player_manager>( _player_manager.lock( ) );
    pla_mgr->packet_loss_completion( [ this ] (vec2 pos) 
    {
        create_feed( pos );
    } );
    auto pla = pla_mgr->get_player( );

    for ( auto& f : _children )
    {
        auto fee = std::dynamic_pointer_cast<feed>( f );
        if ( fee->is_captureing( ) ) continue;

        auto this_pos = fee->get_position( );
        auto this_radius = fee->get_radius( );

        auto target_pos = pla->get_position( );
        auto target_radius = pla->get_radius( );

        // 自分の半径の二倍分の距離から吸い取れます。
        if ( distance( this_pos, target_pos ) < this_radius * 2 + target_radius )
        {
            fee->captured( pla );
        }
    }
}
cinder::vec2 feed_manager::create_feed( )
{
    auto size = _ground.lock( )->get_content_size( ) * _ground.lock( )->get_scale( );

    // エサの作成。
    auto f = feed::create( );
    f->set_tag( _now_tag++ );
    f->set_position( vec2( randFloat( size.x ), randFloat( size.y ) ) );
    add_child( f );

    return f->get_position( );
}
void user::feed_manager::create_feed( cinder::vec2 const& position )
{
    // エサの作成。
    auto f = feed::create( );
    f->set_tag( _now_tag++ );
    f->set_position( position );
    add_child( f );
}
}

