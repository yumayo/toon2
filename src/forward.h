#pragma once
#include <memory>

#define LUA_SETUP_H(__TYPE__) static void lua_setup( sol::state& lua )
#define LUA_SETUP_CPP(__TYPE__) void __TYPE__::lua_setup( sol::state& lua )
#define CREATE_H(__TYPE__, ...) static std::shared_ptr<__TYPE__> create(__VA_ARGS__)
#define CREATE_CPP(__TYPE__, ...) std::shared_ptr<__TYPE__> __TYPE__::create(__VA_ARGS__)
#define CREATE(__TYPE__, ...) \
auto ret = std::make_shared<__TYPE__>( ); \
if ( ret && ret->init( __VA_ARGS__ ) ); \
else ret.reset( ); \
return std::move( ret )
#ifdef assert_disable
#define assert_log(cond, msg, return_value) ((void)0)
#else
#define assert_log(cond, msg, return_value) \
do \
{ \
    if ( !( cond ) ) \
    { \
        log( "Assert failed: %s", msg ); \
        assert( cond ); \
        return_value; \
    } \
} while ( 0 )
#endif

class node;
using node_weak = std::weak_ptr<node>;

namespace action
{
class action;
using action_weak = std::weak_ptr<action>;
}
