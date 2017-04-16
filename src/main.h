#pragma once
#include "user/title.h"
#include "scene_manager.h"
inline void entry_point( )
{
    scene_manager::get_instans( )->push( user::title::create( ) );
}