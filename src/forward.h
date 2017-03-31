#pragma once
#include <memory>

#define CREATE_H(__TYPE__, ...) static std::shared_ptr<__TYPE__> create(__VA_ARGS__)
#define CREATE_CPP(__TYPE__, ...) std::shared_ptr<__TYPE__> __TYPE__::create(__VA_ARGS__)
#define CREATE(__TYPE__, ...) \
auto ret = std::make_shared<__TYPE__>( ); \
if ( ret && ret->init( __VA_ARGS__ ) ); \
else ret.reset( ); \
return std::move( ret )

class node;
