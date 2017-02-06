#pragma once
#include "action.h"
namespace action
{
class repeat_forever : public action
{
public:
    CREATE_H( repeat_forever );
public:
    bool init( );
public:
    virtual bool is_done( ) override;
private:
    // TODO: repeatforever
};
}