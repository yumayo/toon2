#pragma once
#include "../node.h"
namespace user
{
class player_manager : public node
{
public:
    CREATE_H( player_manager );
    bool init( );
};
}