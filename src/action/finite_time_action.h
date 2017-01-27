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
    virtual bool is_done( ) override;
    virtual void update( float delta ) override;

protected:
    float _time = 0.0F;
public:
    void set_time( float value );
    float get_time( );

protected:
    float _duration = 0.0F;
public:
    void set_duration( float value );
    float get_duration( );
};
}