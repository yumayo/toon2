#include "audio.hpp"
#include "buffer_player.h"
#include "file_player.h"
namespace audio
{
void lua_setup( sol::state & lua )
{
    buffer_player::lua_setup( lua );
    file_player::lua_setup( lua );
}
}