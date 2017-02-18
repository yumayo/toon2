#pragma once
#include "cocoslike.h"
namespace user
{
class title : public node
{
public:
    CREATE_H( title );
    bool init( );
private:
    sol::state* lua;
};
}