#pragma once
#include "../node.h"
namespace user
{
class captured_object : public node
{
public:
    virtual void on_captured( std::weak_ptr<node> other ) = 0;
    void captured( std::weak_ptr<node> other );
    bool is_captureing( );
protected:
    bool _captureing = false;
};
}