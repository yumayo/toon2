#pragma once
#include "finite_time_action.h"
namespace action
{
class timeline : public finite_time_action
{
public:
    CREATE_H( timeline );
    virtual ~timeline( ) { }
public:
    bool init( );

protected:
    std::vector<std::shared_ptr<finite_time_action>> _actions;
};
}