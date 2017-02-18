#pragma once
#include "../forward.h"
#include "sol.hpp"
namespace utility
{
sol::state* lmake( std::string const& relative_path, std::shared_ptr<node> const& root );
}