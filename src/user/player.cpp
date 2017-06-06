#include "player.h"
#include "action.hpp"
#include "utility/string_utility.h"
#include "skin.h"
#include "title.h"
using namespace cinder;
namespace user
{
CREATE_CPP( player, std::string const& ip_address,
            int port, std::string const& relative_path_skin )
{
    CREATE( player, ip_address, port, relative_path_skin );
}
bool player::init( std::string const& ip_address,
                   int port, std::string const& relative_path_skin )
{
    _handle = std::make_shared<network::network_object>( ip_address, port );

    _setup_radius = 20.0F;

    set_scale( vec2( 0 ) );
    run_action( action::ease<EaseOutSine>::create( action::scale_to::create( 1.0F, vec2( 1 ) ) ) );

    _radius = _setup_radius;
    if ( auto base = renderer::circle::create( _radius ) )
    {
        _base = base;
        add_child( base );

        if ( auto mask = relative_path_skin.empty( ) ?
             renderer::circle::create( _radius ) : skin::create( _radius, relative_path_skin ) )
        {
            _mask = mask;
            mask->set_scale( vec2( 0.9F ) );
            base->add_child( mask );
        }
    }

    return true;
}
player::~player( )
{
    if ( get_name( ) == "own" )
    {
        auto feed_num = user_default::get_instans( )->get_root( )["feed"].asInt( );
        user_default::get_instans( )->get_root( )["feed"] = feed_num + int( _radius - _setup_radius );
    }
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
    if ( get_name( ) == "own" )
    {
        scene_manager::get_instans( )->replace( title::create( ) );
    }
}
void player::capture( float score )
{
    remove_action_by_name( "capture_animation" );

    // どの大きさまで大きくなるのかを設定する。
    _target_radius = _radius + score;

    // アクションは終了済み。
    auto sub = _target_radius - _radius;
    sub = clamp( sub, 0.0F, 2.0F );
    auto act = action::ease<EaseOutSine>::create( action::float_to::create( sub, _radius, _target_radius, [ this ] ( float value )
    {
        set_radius( value );
    }
    ) );
    act->set_name( "capture_animation" );
    run_action( act );
}
void player::move( cinder::vec2 axis )
{
    cinder::vec2 speed = axis * 300.0F; // 一秒間あたりの速さ。
    speed = speed * ( 1.0F - easeInOutSine( cinder::clamp( ( _radius - _setup_radius ) / 200.0F, 0.0F, 0.8F ) ) );
    set_position( get_position( ) + speed );
}
void user::player::set_color( cinder::ColorA value )
{
    node::set_color( value );
    _mask.lock( )->set_color( value );
}
network::network_handle user::player::get_handle( )
{
    return _handle;
}
}
