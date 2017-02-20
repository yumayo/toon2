#pragma once
#include <string>
#include "cinder/app/App.h"
namespace utility
{
std::string format( char const* format, ... );
void log( char const* str, ... );
void log_data( char const * data, size_t size );
}
namespace cinder
{
namespace app
{
std::string loadString( std::string const& relative_path );
}
}