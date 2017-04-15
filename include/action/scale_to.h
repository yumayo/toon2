#pragma once
#include "finite_time_action.h"
#include "cinder/Vector.h"
namespace action
{
class scale_to : public finite_time_action
{
public:
    CREATE_H( scale_to, float duration, cinder::vec2 scale );
    bool init( float duration, cinder::vec2 scale );
private:
    void setup( ) override;
    void step( float t ) override;
protected:
    cinder::vec2 _start_scale;
    cinder::vec2 _scale;
};
}