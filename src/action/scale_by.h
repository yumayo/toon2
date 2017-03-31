#pragma once
#include "scale_to.h"
namespace action
{
class scale_by : public scale_to
{
public:
    CREATE_H( scale_by, float duration, cinder::vec2 scale );
    bool init( float duration, cinder::vec2 scale );
private:
    void setup( ) override;
private:
    cinder::vec2 _init_scale;
};
}