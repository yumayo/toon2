#pragma once
#include "timeline.h"
namespace action
{
class sequence : public timeline
{
public:
    template<class Head, class... Tail>
    CREATE_H( sequence, Head const& head, Tail const& ...tail );
    bool init( );
    template <class Head, class... Tail>
    bool init( Head const& head, Tail const& ...tail );
public:
    void setup( ) override;
    bool is_done( ) override;
    void update( float delta ) override;
public:
    CREATE_H( sequence, sol::variadic_args const& args );
    bool init( sol::variadic_args const& args );
private:
    std::vector<std::shared_ptr<action>>::iterator _target_action;
public:
    LUA_SETUP_H( sequence );
};

template<class Head, class... Tail>
inline CREATE_CPP( sequence, Head const& head, Tail const& ...tail )
{
    CREATE( sequence, head, tail... );
}

template<class Head, class... Tail>
inline bool sequence::init( Head const& head, Tail const& ...tail )
{
    _actions.emplace_back( head );
    return init( tail... );
}
}