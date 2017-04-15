#pragma once
#include "node.h"
#include <cinder/audio/audio.h>
namespace audio
{
class file_player : public node
{
public:
    CREATE_H( file_player, std::string const& );
    ~file_player( );
    bool init( std::string const& relative_path );
    void play( );
    void stop( );
    void set_gain( float value );
    float get_gain( );
private:
    cinder::audio::GainNodeRef _gain;
    cinder::audio::FilePlayerNodeRef _file_player_node;
};
}