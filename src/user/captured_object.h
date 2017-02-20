#pragma once
#include "../node.h"
namespace user
{
class captured_object : public node
{
public:
    virtual void on_captured( std::weak_ptr<node> other ) = 0;
    virtual void captured( std::weak_ptr<node> other );
    virtual bool captureing( );
protected:
    bool _captureing = false;
};
}