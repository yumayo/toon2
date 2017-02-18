#include "string_utility.h"
#include <stdarg.h>
namespace utility
{
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
    const int max_string_length = ( 1024 * 100 );
    va_list args;
    va_start( args, str );
    char buf[max_string_length];
    vsnprintf( buf, max_string_length, str, args );
    if ( cinder::app::isMainThread( ) )
    {
        // 複数のスレッドから同時にconsoleにアクセスすると壊れてしまいます。
        cinder::app::console( ) << buf << std::endl;
    }
    va_end( args );
}
}

namespace cinder
{
namespace app
{
std::string loadString( std::string const & relative_path )
{
    return static_cast<char*>( loadAsset( relative_path )->getBuffer( )->getData( ) );
}
}
}
