#pragma once
#include "finite_time_action.h"
#include "cinder/Vector.h"
namespace action
{
class fade_out : public finite_time_action
{
public:
    CREATE_H( fade_out, float duration );
    bool init( float duration );
private:
    void setup( ) override;
    void step( float t ) override;
private:
    float _opacity = 0.0F;
};
}