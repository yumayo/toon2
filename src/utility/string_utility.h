#pragma once
#include <string>
#include "cinder/app/App.h"

std::string format( char const* format, ... );
void log( char const* str, ... );

namespace cinder
{
namespace app
{
std::string loadString( std::string const& relative_path );
}
}
