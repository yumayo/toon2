#pragma once
#include "finite_time_action.h"
namespace action
{
class repeat_forever : public finite_time_action
{
public:
    CREATE_H( repeat_forever, std::shared_ptr<finite_time_action> const& );
public:
    bool init( std::shared_ptr<finite_time_action> const& time_action );
public:
    virtual void setup( ) override;
    virtual bool is_done( ) override;
    virtual float update( float delta ) override;
    virtual void restart( ) override;
protected:
    std::shared_ptr<finite_time_action> _time_action;
};
}