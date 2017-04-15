#pragma once
#include "finite_time_action.h"
#include "cinder/Vector.h"
namespace action
{
class rotate_to : public finite_time_action
{
public:
    CREATE_H( rotate_to, float duration, float rotation );
    bool init( float duration, float rotation );
private:
    void setup( ) override;
    void step( float t ) override;
protected:
    float _start_rotation = 0.0F;
    float _rotation = 0.0F;
};
}