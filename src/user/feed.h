#pragma once
#include "../renderer/sprite.h"
namespace user
{
class feed : public renderer::sprite
{
public:
    CREATE_H( feed );
    bool init( );
public:
    float capture( );
private:
    float _score = 0.0F;
};
}