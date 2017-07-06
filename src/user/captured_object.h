#pragma once
#include <treelike/node.h>
namespace user
{
class captured_object : public treelike::node
{
public:
    virtual void on_captured( softptr<treelike::node> other ) = 0;
    virtual ~captured_object( );
    void captured( softptr<treelike::node> other );
    bool is_captureing( );
protected:
    bool _captureing = false;
};
}