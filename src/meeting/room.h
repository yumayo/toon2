#pragma once
#include "cocoslike.h"
namespace meeting
{
class room : public node
{
    struct _member;
    std::shared_ptr<_member> _m;
public:
    room( );
    CREATE_H( room );
    bool init( );
};
}