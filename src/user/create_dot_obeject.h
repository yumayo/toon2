#pragma once
#include <treelike/node.h>
#include "button.h"
namespace user
{
class dot_object : public treelike::node
{
public:
    CREATE_H( dot_object, std::string const& relative_path, float target_width );
    bool init( std::string const& relative_path, float target_width );
};
class dot_button : public rect_button_scale_action
{
public:
    CREATE_H( dot_button, std::string const& relative_path, float target_width );
    bool init( std::string const& relative_path, float target_width );
};
}