#pragma once
#include "rotate_to.h"
namespace action
{
class rotate_by : public rotate_to
{
public:
    CREATE_H( rotate_by, float duration, float rotation );
    bool init( float duration, float rotation );
private:
    void setup( ) override;
private:
    float _init_rotation = 0.0F;
};
}