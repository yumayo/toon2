#pragma once
#include <list>
#include "ref.h"
class auto_release_pool
{
public:
    static auto_release_pool* get_instans( );
    static void remove_instans( );
    void add_child( ref* object );
    void collect_garbage( );
private:
    auto_release_pool( );
    static auto_release_pool* _instans;
private:
    std::list<ref*> _objects;
};