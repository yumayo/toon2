#include "cell.h"
#include <treelike/action.hpp>
#include "enemy.h"
#include "cinder/gl/Texture.h"
#include <treelike/scene_manager.h>
using namespace cinder;
using namespace treelike;
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
void enemy::on_captured( softptr<node> other )
{
    // nothing
}
}
