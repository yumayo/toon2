#pragma once
#include "../node.h"
namespace user
{
class field : public node
{
public:
    CREATE_H( field );
    bool init( );
};
}