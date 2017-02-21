#pragma once
#include "cinder/Vector.h"
#include <deque>
#include <utility>
#include <vector>
#include <mutex>
namespace user
{
struct player_data
{
    cinder::vec2 position;
    float radius = 0.0F;
};
struct captured_feed_data : public
    std::vector<std::pair<int, cinder::vec2>>
{
};

// 常に先頭が新しいデータです。
class toon_packet
{
public:
    toon_packet( );
    void update( );
    void data_updated( );
    void set_player_data( cinder::vec2 positoin, float radius );
    void add_captured_feed( std::pair<int, cinder::vec2> const& spawn_position );
    void get_data( char* value );
    void set_data( char const* data );
    player_data const& get_player_data( );
    size_t size( );
    size_t player_data_size( );
    size_t captured_feed_data_size( );
    void lock( );
public:
    std::function<void( cinder::vec2 data )> on_packet_loss;
private:
    player_data _player_data;

    // パケットロスをする可能性があるので、_sizeフレーム前までのデータを詰めます。
    std::deque<captured_feed_data> _captured_feed_data;
    static const size_t _size;

    size_t _update_frame = 0;
    size_t _data_updated_frame = 0;

    size_t _packet_loss_frame = 0;

    std::mutex lock_object;
};
}