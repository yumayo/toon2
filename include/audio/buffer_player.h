#pragma once
#include "node.h"
#include <cinder/audio/audio.h>
namespace audio
{
class buffer_player : public node
{
public:
    CREATE_H( buffer_player, std::string const& );
    ~buffer_player( );
    bool init( std::string const& relative_path );
    void play( );
    void stop( );
    void set_gain( float value );
    float get_gain( );
private:
    cinder::audio::GainNodeRef _gain;
    cinder::audio::BufferPlayerNodeRef _buffer_player_node;
};
}