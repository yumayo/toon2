#include "string_utility.h"
#include <stdarg.h>

std::string format( char const * str, ... )
{
    const int max_string_length = ( 1024 * 100 );
    std::string ret;
    va_list args;
    va_start( args, str );
    char buf[max_string_length];
    vsnprintf( buf, max_string_length, str, args );
    ret = buf;
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
    vsnprintf( buf, max_string_length, str, args );
    cinder::app::console( ) << buf << std::endl;
    va_end( args );
    #endif
}

namespace cinder
{
namespace app
{
std::string cinder::app::loadString( std::string const & relative_path )
{
    return std::move( std::string( static_cast<char*>( loadAsset( relative_path )->getBuffer( )->getData( ) ) ) );
}
}
}