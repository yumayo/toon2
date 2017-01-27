#pragma once
#include "action.h"
namespace action
{
class finite_time_action : public action
{
public:
    CREATE_H( finite_time_action );

public:
    bool init( );
public:
    virtual bool is_done( );
    virtual void update( float delta );

protected:
    float _time = 0.0F;

protected:
    float _duration = 0.0F;
};
}