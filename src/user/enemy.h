#pragma once
#include "cell.h"
namespace user
{
class enemy : public cell
{
public:
    CREATE_H( enemy, std::string const& ip_address,
              int port, std::string const& relative_path_skin = "" );
    bool init( std::string const& ip_address, int port, std::string const& relative_path_skin );
public:
    void on_captured( std::weak_ptr<node> other ) override;
};
}