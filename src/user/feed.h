#pragma once
#include "../renderer/circle.h"
#include "captured_object.h"
namespace user
{
class feed : public captured_object
{
public:
    CREATE_H( feed );
    bool init( );
public:
    float get_radius( );
    void on_captured( std::weak_ptr<node> other );
private:
    std::weak_ptr<renderer::circle> _circle;
    float _score = 0.0F;
};
}