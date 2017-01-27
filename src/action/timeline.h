#pragma once
#include "action.h"
namespace action
{
class timeline : public action
{
public:
    CREATE_H( timeline );
    bool init( );

protected:
    std::vector<std::shared_ptr<action>> _actions;
};
}