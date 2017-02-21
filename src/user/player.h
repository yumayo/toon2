#pragma once
#include "captured_object.h"
#include "../renderer/circle.h"
#include "toon_packet.h"
namespace user
{
class player : public captured_object
{
public:
    CREATE_H( player, cinder::ColorA color );
    bool init( cinder::ColorA color );
public:
    float get_radius( );
    void set_radius( float value );
    void on_captured( std::weak_ptr<node> other );
    void capture( float score );
protected:
    std::weak_ptr<renderer::circle> _base;
    std::weak_ptr<renderer::circle> _mask;
    float _radius = 0.0F;
    float _target_radius = 0.0F;
};
}