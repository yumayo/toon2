﻿#include "sprite_animation.h"
#include "utility/assert_log.h"
#include "cinder/gl/gl.h"
using namespace cinder;
namespace renderer
{
CREATE_CPP( sprite_animation, std::string const& relative_path )
{
    CREATE( sprite_animation, relative_path );
}
sprite_animation::~sprite_animation( )
{
}
bool sprite_animation::init( std::string const& relative_path )
{
    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );
    set_schedule_update( );

    assert_log( !app::getAssetPath( relative_path ).empty( ), "ファイルが見つかりません。", return false );

    _texture = gl::Texture::create( loadImage( app::loadAsset( relative_path ) ) );

    return true;
}
void sprite_animation::update( float delta )
{
    _animation_time += delta * _framerate;
}
void sprite_animation::render( )
{
    int animation_index = std::fmodf( _animation_time, _cut_x * _cut_y );
    float x_offset = ( animation_index % _cut_x ) * _cut_size.x;
    float y_offset = ( animation_index / _cut_x ) * _cut_size.y;
    gl::draw( _texture, Area( vec2( x_offset, y_offset ), vec2( x_offset, y_offset ) + _cut_size ), Rectf( vec2( 0 ), _content_size ) );
}
cinder::vec2 sprite_animation::get_cut_size( )
{
    return _cut_size;
}
void sprite_animation::set_cut_size( cinder::vec2 value )
{
    _cut_size = value;
    _content_size = _cut_size;
}
int sprite_animation::get_cut_x( )
{
    return _cut_x;
}
void sprite_animation::set_cut_x( int value )
{
    _cut_x = value;
}
int sprite_animation::get_cut_y( )
{
    return _cut_y;
}
void sprite_animation::set_cut_y( int value )
{
    _cut_y = value;
}
cinder::ivec2 sprite_animation::get_cut( )
{
    return cinder::ivec2( _cut_x, _cut_y );
}
void sprite_animation::set_cut( cinder::ivec2 value )
{
    _cut_x = value.x;
    _cut_y = value.y;
}
float sprite_animation::get_framerate( )
{
    return _framerate;
}
void sprite_animation::set_framerate( float value )
{
    _framerate = value;
}
}