#pragma once
#include "button.h"
#include "rect.h"
#include "sprite.h"
#include "label.h"
namespace renderer
{
void lua_setup( sol::state& lua );

inline void lua_setup( sol::state& lua )
{
    button::lua_setup( lua );
    rect::lua_setup( lua );
    sprite::lua_setup( lua );
    label::lua_setup( lua );
}
}
