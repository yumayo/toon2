#pragma once
#include "player.h"
namespace user
{
class enemy : public player
{
public:
    CREATE_H( enemy );
    bool init( );
};
}