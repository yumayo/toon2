#pragma once
#include "../renderer/circle.h"
namespace user
{
class feed : public renderer::circle
{
public:
    CREATE_H( feed );
    bool init( );
public:
    void captured( std::weak_ptr<node> player );
    bool captureing( );
private:
    bool _captureing = false;
    float _score = 0.0F;
};
}