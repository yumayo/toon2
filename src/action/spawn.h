#pragma once
#include "timeline.h"
namespace action
{
class spawn : public timeline
{
public:
    template<class Head, class... Tail>
    CREATE_H( spawn, Head const& head, Tail const& ...tail );
    bool init( );
    template <class Head, class... Tail>
    bool init( Head const& head, Tail const& ...tail );
public:
    CREATE_H( spawn, sol::variadic_args const& args );
    bool init( sol::variadic_args const& args );
public:
    void setup( ) override;
    bool is_done( ) override;
    void update( float delta ) override;
public:
    LUA_SETUP_H( spawn );
};

template<class Head, class... Tail>
inline CREATE_CPP( spawn, Head const& head, Tail const& ...tail )
{
    CREATE( spawn, head, tail... );
}

template<class Head, class... Tail>
inline bool spawn::init( Head const& head, Tail const& ...tail )
{
    _actions.emplace_back( head );
    return init( tail... );
}
}