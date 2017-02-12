﻿#include "lua_setup_cinder.h"
#include "forward.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"
namespace cinder
{
void lua_setup( sol::state & lua )
{
    lua.new_usertype<vec2>( "vec2"
                            , sol::constructors< sol::types<>
                            , sol::types<vec2>
                            , sol::types<float>
                            , sol::types<float, float>
                            >( )
                            , "x", &vec2::x
                            , "y", &vec2::y
                            );
    lua.new_usertype<ColorA>( "color"
                              , sol::constructors< sol::types<>
                              , sol::types<ColorA>
                              , sol::types<float, float, float, float>
                              >( )
                              , "r", &ColorA::r
                              , "g", &ColorA::g
                              , "b", &ColorA::b
                              , "a", &ColorA::a
                              );
}
}