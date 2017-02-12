#pragma once
#include "delay.h"
#include "ease.h"
#include "float_to.h"
#include "move_to.h"
#include "remove_self.h"
#include "sequence.h"
#include "spawn.h"
#include "repeat_forever.h"

namespace action
{
void lua_setup( sol::state& lua );

inline void lua_setup( sol::state& lua )
{
    action::lua_setup( lua );

    finite_time_action::lua_setup( lua );

    timeline::lua_setup( lua );

    float_to::lua_setup( lua );

    sequence::lua_setup( lua );
    spawn::lua_setup( lua );

    delay::lua_setup( lua );

    remove_self::lua_setup( lua );

    move_to::lua_setup( lua );

    repeat_forever::lua_setup( lua );

    ease<cinder::EaseNone>::lua_setup( lua );
    ease<cinder::EaseInQuad>::lua_setup( lua );
    ease<cinder::EaseOutQuad>::lua_setup( lua );
    ease<cinder::EaseInOutQuad>::lua_setup( lua );
    ease<cinder::EaseOutInQuad>::lua_setup( lua );
    ease<cinder::EaseInCubic>::lua_setup( lua );
    ease<cinder::EaseOutCubic>::lua_setup( lua );
    ease<cinder::EaseInOutCubic>::lua_setup( lua );
    ease<cinder::EaseOutInCubic>::lua_setup( lua );
    ease<cinder::EaseInQuart>::lua_setup( lua );
    ease<cinder::EaseOutQuart>::lua_setup( lua );
    ease<cinder::EaseInOutQuart>::lua_setup( lua );
    ease<cinder::EaseOutInQuart>::lua_setup( lua );
    ease<cinder::EaseInQuint>::lua_setup( lua );
    ease<cinder::EaseOutQuint>::lua_setup( lua );
    ease<cinder::EaseInOutQuint>::lua_setup( lua );
    ease<cinder::EaseOutInQuint>::lua_setup( lua );
    ease<cinder::EaseInSine>::lua_setup( lua );
    ease<cinder::EaseOutSine>::lua_setup( lua );
    ease<cinder::EaseInOutSine>::lua_setup( lua );
    ease<cinder::EaseOutInSine>::lua_setup( lua );
    ease<cinder::EaseInExpo>::lua_setup( lua );
    ease<cinder::EaseOutExpo>::lua_setup( lua );
    ease<cinder::EaseInOutExpo>::lua_setup( lua );
    ease<cinder::EaseOutInExpo>::lua_setup( lua );
    ease<cinder::EaseInCirc>::lua_setup( lua );
    ease<cinder::EaseOutCirc>::lua_setup( lua );
    ease<cinder::EaseInOutCirc>::lua_setup( lua );
    ease<cinder::EaseOutInCirc>::lua_setup( lua );
    ease<cinder::EaseInBounce>::lua_setup( lua );
    ease<cinder::EaseOutBounce>::lua_setup( lua );
    ease<cinder::EaseInOutBounce>::lua_setup( lua );
    ease<cinder::EaseOutInBounce>::lua_setup( lua );
    ease<cinder::EaseInBack>::lua_setup( lua );
    ease<cinder::EaseOutBack>::lua_setup( lua );
    ease<cinder::EaseInOutBack>::lua_setup( lua );
    ease<cinder::EaseOutInBack>::lua_setup( lua );
    ease<cinder::EaseInAtan>::lua_setup( lua );
    ease<cinder::EaseOutAtan>::lua_setup( lua );
    ease<cinder::EaseInOutAtan>::lua_setup( lua );
}
}
