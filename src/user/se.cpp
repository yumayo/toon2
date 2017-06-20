#include "se.h"
#include "audio.hpp"
#include "scene_manager.h"
void user::add_se( std::string const & relative_path, float gain )
{
    if ( !scene_manager::get_instans( )->get_dont_destroy_node( ).lock( )->get_child_by_name( relative_path ) )
    {
        auto se = audio::buffer_player::create( relative_path );
        se->set_name( relative_path );
        se->set_gain( gain );
        scene_manager::get_instans( )->get_dont_destroy_node( ).lock( )->add_child( se );
    }
}
void user::play_se( std::string const & relative_path )
{
    std::dynamic_pointer_cast<::audio::buffer_player>
        ( scene_manager::get_instans( )->get_dont_destroy_node( ).lock( )->get_child_by_name( relative_path ) )->play( );
}
