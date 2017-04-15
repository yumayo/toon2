#pragma once
#include "finite_time_action.h"
namespace action
{
class call_func : public finite_time_action
{
public:
    CREATE_H( call_func, std::function<void( )> call );
    bool init( std::function<void( )> call );
public:
    virtual void update( float delta ) override;
private:
    std::function<void( )> _call;
};
}