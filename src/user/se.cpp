#include "se.h"
#include <treelike/audio.hpp>
#include <treelike/scene_manager.h>
using namespace treelike;
void user::add_se( std::string const & relative_path, float gain )
{
    if ( !scene_manager::get_instans( )->get_dont_destroy_node( )->get_child_by_name( relative_path ) )
    {
        auto se = audio::buffer_player::create( relative_path );
        se->set_name( relative_path );
        se->set_gain( gain );
        scene_manager::get_instans( )->get_dont_destroy_node( )->add_child( se );
    }
}
void user::play_se( std::string const & relative_path )
{
    scene_manager::get_instans( )->get_dont_destroy_node( )->get_child_by_name( relative_path ).dynamicptr<treelike::audio::buffer_player>( )->play( );
}
