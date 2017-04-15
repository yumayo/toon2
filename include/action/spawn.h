#pragma once
#include "timeline.h"
namespace action
{
// �����̃A�N�V���������Ɏ��s���܂��B
// final class
class spawn : public timeline
{
public:
    template<class Head, class... Tail>
    CREATE_H( spawn, Head const& head, Tail const& ...tail );
    bool init( );
    template <class Head, class... Tail>
    bool init( Head const& head, Tail const& ...tail );
public:
    void setup( ) final override;
    bool is_done( )  final override;
    float update( float delta )  final override;
    void restart( )  final override;
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