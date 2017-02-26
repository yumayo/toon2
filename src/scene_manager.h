#pragma once
#include "scene.h"
#include <stack>
class scene_manager
{
public:
    void replace( std::shared_ptr<scene> const& scene );
    void push( std::shared_ptr<scene> const& scene );
    void pop( );
    size_t size( );
    bool empty( );
    std::shared_ptr<scene>& top( );
    void update( );
private:
    std::vector<std::function<void( )>> _fn;
    std::stack<std::shared_ptr<scene>> _stack;
public:
    static scene_manager* get_instans( );
    static void remove_instans( );
private:
    scene_manager( );
    static scene_manager* _instans;
};