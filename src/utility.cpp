#include "utility.h"
#include <stdarg.h>
using namespace cinder;

std::string format( char const * str, ... )
{
    const int max_string_length = ( 1024 * 100 );
    std::string ret;
    va_list args;
    va_start( args, str );
    char buf[max_string_length];
    if ( buf != nullptr )
    {
        vsnprintf_s( buf, max_string_length, str, args );
        ret = buf;
    }
    va_end( args );
    return ret;
}

void log( char const * str, ... )
{
    #ifdef _DEBUG
    const int max_string_length = ( 1024 * 100 );
    va_list args;
    va_start( args, str );
    char buf[max_string_length];
    if ( buf != nullptr )
    {
        vsnprintf_s( buf, max_string_length, str, args );
        app::console( ) << buf << std::endl;
    }
    va_end( args );
    #endif
}

int hit_point_polygon_2d( cinder::vec2 A, cinder::vec2 B, cinder::vec2 C, cinder::vec2 P )
{
    cinder::vec2 AB = B - A;
    cinder::vec2 BP = P - B;

    cinder::vec2 BC = C - B;
    cinder::vec2 CP = P - C;

    cinder::vec2 CA = A - C;
    cinder::vec2 AP = P - A;

    double c1 = AB.x * BP.y - AB.y * BP.x;
    double c2 = BC.x * CP.y - BC.y * CP.x;
    double c3 = CA.x * AP.y - CA.y * AP.x;

    if ( c1 >= 0.0 && c2 >= 0.0 && c3 >= 0.0 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
