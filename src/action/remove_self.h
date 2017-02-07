#pragma once
#include "action.h"
namespace action
{
class remove_self : public action
{
public:
    CREATE_H( remove_self );

public:
    bool init( );
public:
    void update( float delta ) override;
public:
    LUA_SETUP_H( remove_self );
};
}