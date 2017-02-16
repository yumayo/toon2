#pragma once
#include "repeat_forever.h"
namespace action
{
class repeat_times : public repeat_forever
{
public:
    CREATE_H( repeat_times, std::shared_ptr<finite_time_action>, int );
public:
    bool init( std::shared_ptr<finite_time_action> time_action, int number_of_times );
public:
    virtual bool is_done( ) override;
    virtual void update( float delta ) override;
    virtual void restart( ) override;
private:
    int _init_number_of_times = 0;
    int _number_of_times = 0;
public:
    LUA_SETUP_H( repeat_times );
};
}