#pragma once
#include "captured_object.h"
#include "../renderer/circle.h"
#include "../network/udp_sender.h"
namespace user
{
class player : public captured_object
{
public:
    CREATE_H( player, cinder::ColorA color );
    bool init( cinder::ColorA color );
    void update( float delta )override;
public:
    float get_radius( );
    void on_captured( std::weak_ptr<node> other );
    void capture( float score );
private:
    std::weak_ptr<renderer::circle> _base;
    std::weak_ptr<renderer::circle> _mask;
    float _radius = 0.0F;
    float _target_radius = 0.0F;
    std::weak_ptr<network::udp_sender> _sender;
};
}