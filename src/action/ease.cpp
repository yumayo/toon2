#include "ease.h"
using namespace cinder;
namespace action
{
#define l_ease( __name) \
LUA_SETUP_CPP( l_class )\
{\
    l_new( __name\
           , l_base( finite_time_action )\
           , l_set( create )\
    );\
}


template<>
#define l_class ease<EaseNone>
#include "lua_define.h"
l_ease( EaseNone )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInQuad>
#include "lua_define.h"
l_ease( EaseInQuad )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutQuad>
#include "lua_define.h"
l_ease( EaseOutQuad )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInOutQuad>
#include "lua_define.h"
l_ease( EaseInOutQuad )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutInQuad>
#include "lua_define.h"
l_ease( EaseOutInQuad )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInCubic>
#include "lua_define.h"
l_ease( EaseInCubic )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutCubic>
#include "lua_define.h"
l_ease( EaseOutCubic )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInOutCubic>
#include "lua_define.h"
l_ease( EaseInOutCubic )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutInCubic>
#include "lua_define.h"
l_ease( EaseOutInCubic )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInQuart>
#include "lua_define.h"
l_ease( EaseInQuart )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutQuart>
#include "lua_define.h"
l_ease( EaseOutQuart )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInOutQuart>
#include "lua_define.h"
l_ease( EaseInOutQuart )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutInQuart>
#include "lua_define.h"
l_ease( EaseOutInQuart )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInQuint>
#include "lua_define.h"
l_ease( EaseInQuint )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutQuint>
#include "lua_define.h"
l_ease( EaseOutQuint )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInOutQuint>
#include "lua_define.h"
l_ease( EaseInOutQuint )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutInQuint>
#include "lua_define.h"
l_ease( EaseOutInQuint )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInSine>
#include "lua_define.h"
l_ease( EaseInSine )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutSine>
#include "lua_define.h"
l_ease( EaseOutSine )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInOutSine>
#include "lua_define.h"
l_ease( EaseInOutSine )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutInSine>
#include "lua_define.h"
l_ease( EaseOutInSine )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInExpo>
#include "lua_define.h"
l_ease( EaseInExpo )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutExpo>
#include "lua_define.h"
l_ease( EaseOutExpo )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInOutExpo>
#include "lua_define.h"
l_ease( EaseInOutExpo )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutInExpo>
#include "lua_define.h"
l_ease( EaseOutInExpo )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInCirc>
#include "lua_define.h"
l_ease( EaseInCirc )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutCirc>
#include "lua_define.h"
l_ease( EaseOutCirc )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInOutCirc>
#include "lua_define.h"
l_ease( EaseInOutCirc )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutInCirc>
#include "lua_define.h"
l_ease( EaseOutInCirc )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInBounce>
#include "lua_define.h"
l_ease( EaseInBounce )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutBounce>
#include "lua_define.h"
l_ease( EaseOutBounce )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInOutBounce>
#include "lua_define.h"
l_ease( EaseInOutBounce )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutInBounce>
#include "lua_define.h"
l_ease( EaseOutInBounce )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInBack>
#include "lua_define.h"
l_ease( EaseInBack )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutBack>
#include "lua_define.h"
l_ease( EaseOutBack )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInOutBack>
#include "lua_define.h"
l_ease( EaseInOutBack )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutInBack>
#include "lua_define.h"
l_ease( EaseOutInBack )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInAtan>
#include "lua_define.h"
l_ease( EaseInAtan )
#include "lua_undef.h"

template<>
#define l_class ease<EaseOutAtan>
#include "lua_define.h"
l_ease( EaseOutAtan )
#include "lua_undef.h"

template<>
#define l_class ease<EaseInOutAtan>
#include "lua_define.h"
l_ease( EaseInOutAtan )
#include "lua_undef.h"

// 何故かEaseOutInAtanはありません。

#undef l_ease
}
