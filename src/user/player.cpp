#include "player.h"
#include "../action/action.hpp"
#include "../utility/string_utility.h"
using namespace cinder;
namespace user
{
CREATE_CPP( player, cinder::ColorA color )
{
    CREATE( player, color );
}
bool player::init( cinder::ColorA color )
{
    set_color( color );

    _setup_radius = 20.0F;

    _radius = _setup_radius;
    if ( auto base = renderer::circle::create( _radius ) )
    {
        _base = base;
        add_child( base );

        if ( auto mask = renderer::circle::create( _radius ) )
        {
            _mask = mask;
            mask->set_color( get_color( ) );
            mask->set_scale( vec2( 0.9F ) );
            base->add_child( mask );
        }
    }

    return true;
}
float player::get_radius( )
{
    return _radius;
}
void player::set_radius( float value )
{
    _radius = value;
    _base.lock( )->set_radius( get_radius( ) );
    _mask.lock( )->set_radius( get_radius( ) );
}
void player::on_captured( std::weak_ptr<node> other )
{
    auto pla = std::dynamic_pointer_cast<user::player>( other.lock( ) );
    pla->capture( _radius );
}
void player::capture( float score )
{
    if ( is_running_action( ) )
    {
        remove_all_actions( );
    }

    // どの大きさまで大きくなるのかを設定する。
    _target_radius = _radius + score;

    // アクションは終了済み。
    auto sub = _target_radius - _radius;
    sub = clamp( sub, 0.0F, 2.0F );
    run_action( action::ease<EaseOutSine>::create( action::float_to::create( sub, _radius, _target_radius, [ this ] ( float value )
    {
        set_radius( value );
    }
    ) ) );
}
void player::move( cinder::vec2 axis )
{
    cinder::vec2 speed = axis * 300.0F; // 一秒間あたりの速さ。
    speed = speed * ( 1.0F - easeInOutSine( cinder::clamp( ( _radius - _setup_radius ) / 200.0F, 0.0F, 0.8F ) ) );
    set_position( get_position( ) + speed );
}
}
