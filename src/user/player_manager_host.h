#pragma once
#include "player_manager.h"
namespace user
{
class player_manager_host : public player_manager
{
public:
    CREATE_H( player_manager_host );
    bool init( );
};
}