#pragma once
#include "cinder/Vector.h"
// 以下のサイトを参考にしました。
// http://www.sousakuba.com/Programming/gs_hittest_point_triangle.html
int hit_point_polygon_2d( cinder::vec2 a, cinder::vec2 b, cinder::vec2 c, cinder::vec2 point );