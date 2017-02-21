#include "toon_packet.h"
#include <vector>
namespace user
{
const size_t toon_packet::_size = 10;
toon_packet::toon_packet( )
{
    _captured_feed_data = std::deque<captured_feed_data>( _size );
}
void toon_packet::update( )
{
    lock( );

    _my_frame++;

    _captured_feed_data.pop_back( );
    _captured_feed_data.emplace_front( captured_feed_data( ) );
}
void toon_packet::data_updated( )
{
    _enemy_frame++;
}
void toon_packet::set_player_data( cinder::vec2 positoin, float radius )
{
    // プレイヤーデータは別に書き換えられても困らないので、mutexにはしません。

    _player_data.position = positoin;
    _player_data.radius = radius;
}
void toon_packet::add_captured_feed( std::pair<int, cinder::vec2> const& data )
{
    lock( );

    auto& cap_data = _captured_feed_data.front( );
    cap_data.emplace_back( data );
}
void toon_packet::get_data( char* value )
{
    lock( );

    memcpy( value, &_player_data, sizeof( player_data ) );
    value += player_data_size( );

    // _sizeフレーム分データを詰めます。
    for ( auto& obj : _captured_feed_data )
    {
        // 一フレーム分のデータを詰める。

        // 配列の要素数。
        int array_size = obj.size( );
        memcpy( value, &array_size, sizeof( int ) );
        value += sizeof( int );

        // 配列の中身。
        for ( auto& d : obj )
        {
            int id = d.first;
            memcpy( value, &id, sizeof( int ) );
            value += sizeof( int );

            cinder::vec2 spawn_position = d.second;
            memcpy( value, &spawn_position, sizeof( cinder::vec2 ) );
            value += sizeof( cinder::vec2 );
        }
    }
}
void toon_packet::set_data( char const * data )
{
    lock( );

    memcpy( &_player_data, data, sizeof( player_data ) );
    data += player_data_size( );

    // _sizeフレーム分データを詰めます。
    for ( int s = 0; s < _size; ++s )
    {
        // 一フレーム分のデータを詰める。
        captured_feed_data temp;

        // 配列の要素数。
        int array_size;
        memcpy( &array_size, data, sizeof( int ) );
        data += sizeof( int );

        // 配列の中身。
        for ( int i = 0; i < array_size; ++i )
        {
            int id;
            memcpy( &id, data, sizeof( int ) );
            data += sizeof( int );

            cinder::vec2 spawn_position;
            memcpy( &spawn_position, data, sizeof( cinder::vec2 ) );
            data += sizeof( cinder::vec2 );

            temp.emplace_back( std::make_pair( id, spawn_position ) );
        }

        _captured_feed_data.pop_back( );
        _captured_feed_data.emplace_front( temp );
    }
}
player_data const & toon_packet::get_player_data( )
{
    // こちらもプレイヤーデータへのアクセスは気にしません。

    return _player_data;
}
size_t toon_packet::size( )
{
    return player_data_size( ) + captured_feed_data_size( );
}
size_t toon_packet::player_data_size( )
{
    return sizeof( player_data );
}
size_t toon_packet::captured_feed_data_size( )
{
    lock( );

    size_t all = 0;
    for ( auto& obj : _captured_feed_data )
    {
        all += sizeof( int );
        all += ( sizeof( int ) + sizeof( cinder::vec2 ) ) * obj.size( );
    }
    return all;
}
void toon_packet::lock( )
{
    std::lock_guard<decltype( lock_object )> lk( lock_object );
}
}