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
private:
    std::vector<action_ref>::iterator _target_action;
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