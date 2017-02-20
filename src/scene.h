#pragma once
#include "node.h"
class scene : public node
{
public:
    virtual ~scene( );
    CREATE_H( scene );
    bool init( );
};