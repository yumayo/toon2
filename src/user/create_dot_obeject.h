#pragma once
#include <treelike/node.h>
#include "button_scale.h"
namespace user
{
class dot_object : public treelike::node
{
public:
    CREATE_H( dot_object, std::string const& relative_path, float target_width );
    bool init( std::string const& relative_path, float target_width );
};
class dot_button : public button_scale
{
public:
    CREATE_H( dot_button, std::string const& relative_path, float target_width );
    bool init( std::string const& relative_path, float target_width );
};
}