#pragma once
#include <treelike.hpp>
#include "button_scale.h"
namespace user
{
std::shared_ptr<treelike::node> create_dot( std::string const& relative_path,
                                            float target_width );
std::shared_ptr<button_scale> create_dot_button( std::string const& relative_path,
                                                 float target_width );
}