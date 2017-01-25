#pragma once
#include <string>
#include "cinder/app/App.h"
#include "cinder/Vector.h"

std::string format( char const* format, ... );
void log( char const* str, ... );

// 以下のサイトを参考にしました。
// http://www.sousakuba.com/Programming/gs_hittest_point_triangle.html
int hit_point_polygon_2d( cinder::vec2 A, cinder::vec2 B, cinder::vec2 C, cinder::vec2 P );