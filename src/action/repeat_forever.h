#pragma once
#include "finite_time_action.h"
namespace action
{
class repeat_forever : public action
{
public:
    CREATE_H( repeat_forever, std::weak_ptr<finite_time_action> );
public:
    bool init( std::weak_ptr<finite_time_action> time_action );
public:
    virtual bool is_done( ) override;
    virtual void update( float delta ) override;
private:
    std::weak_ptr<finite_time_action> _time_action;
};
}