#pragma once
#include "scene.h"
#include <stack>
class scene_manager : public std::stack<std::shared_ptr<scene>>
{
public:
    static scene_manager* get_instans( );
    static void remove_instans( );
private:
    scene_manager( );
    ~scene_manager( );
    static scene_manager* _instans;
};