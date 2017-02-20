#pragma once
#include "../node.h"
#include "../renderer/circle.h"
#include "../network/udp_sender.h"
namespace user
{
class player : public node
{
public:
    CREATE_H( player );
    bool init( );
    void update( float delta )override;
public:
    void set_radius( float value );
    float get_radius( );
    void capture( float score );
private:
    std::weak_ptr<renderer::circle> _base;
    std::weak_ptr<renderer::circle> _mask;
    float _radius = 0.0F;
    float _target_radius = 0.0F;
    std::weak_ptr<network::udp_sender> _sender;
};
}