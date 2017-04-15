#pragma once
namespace network
{
class use_destroy_object
{
protected:
    mutable bool _is_live = true;
public:
    virtual ~use_destroy_object( ) { }
    void destroy( ) const;
    bool is_destroy( ) const;
};
}
