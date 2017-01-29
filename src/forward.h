#pragma once
#include <memory>

#define CREATE_H(__TYPE__, ...) static std::shared_ptr<__TYPE__> create(__VA_ARGS__)
#define CREATE_CPP(__TYPE__, ...) std::shared_ptr<__TYPE__> __TYPE__::create(__VA_ARGS__)
#define CREATE(__TYPE__, ...) \
auto ret = std::make_shared<__TYPE__>( ); \
if ( ret && ret->init( __VA_ARGS__ ) ); \
else ret.reset( ); \
return std::move( ret )
#define assert_log(cond, msg) \
do \
{ \
    if ( !( cond ) ) \
    { \
        log( "Assert failed: %s", msg ); \
        assert( cond ); \
    } \
} while ( 0 )

class node;
using node_weak = std::weak_ptr<node>;

namespace action
{
class action;
using action_weak = std::weak_ptr<action>;
}
