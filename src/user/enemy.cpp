#include "cell.h"
#include "action.hpp"
#include "utility/string_utility.h"
#include "enemy.h"
#include "cinder/gl/Texture.h"
#include "scene_manager.h"
using namespace cinder;
namespace user
{
CREATE_CPP( enemy, std::string const& ip_address,
            int port, std::string const& relative_path_skin )
{
    CREATE( enemy, ip_address, port, relative_path_skin );
}
bool enemy::init( std::string const& ip_address,
                  int port, std::string const& relative_path_skin )
{
    return cell::init( ip_address, port, relative_path_skin );
}
void enemy::on_captured( std::weak_ptr<node> other )
{
    // nothing
}
}
