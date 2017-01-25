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
