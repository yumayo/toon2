#pragma once
#include "cinder/Vector.h"
namespace utility
{
// 以下のサイトを参考にしました。
// http://www.sousakuba.com/Programming/gs_hittest_point_triangle.html
int hit_point_polygon_2d( cinder::vec2 a, cinder::vec2 b, cinder::vec2 c, cinder::vec2 point );

int hit_point_plane_2d( cinder::vec2 a, cinder::vec2 b, cinder::vec2 c, cinder::vec2 d, cinder::vec2 point );

bool hit_segment( cinder::vec2 s1, cinder::vec2 e1, cinder::vec2 s2, cinder::vec2 e2 );

cinder::vec2 get_hit_segment_intersection( cinder::vec2 s1, cinder::vec2 e1, cinder::vec2 s2, cinder::vec2 e2 );
}