#pragma once
#include "../node.h"
#include "../renderer/circle.h"
namespace user
{
class player : public node
{
public:
    CREATE_H( player );
    bool init( );
public:
    void set_radius( float value );
    float get_radius( );
    void predation( float score );
private:
    std::weak_ptr<renderer::circle> _base;
    std::weak_ptr<renderer::circle> _mask;
    float _radius = 0.0F;
};
}