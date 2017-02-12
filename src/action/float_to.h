#pragma once
#include "finite_time_action.h"
namespace action
{
class float_to : public finite_time_action
{
public:
    using float_to_callback = std::function<void( float value )>;
public:
    CREATE_H( float_to, float duration, float from, float to, std::function<void( float value )> callback );
    bool init( float duration, float from, float to, float_to_callback callback );
private:
    void step( float t ) override;
private:
    float_to_callback _callback;
    float _from = 0.0F;
    float _to = 0.0F;
public:
    LUA_SETUP_H( float_to );
};
}