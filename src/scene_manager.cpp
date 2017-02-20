#include "scene_manager.h"
scene_manager* scene_manager::_instans = nullptr;
scene_manager * scene_manager::get_instans( )
{
    if ( !_instans ) _instans = new scene_manager;
    return _instans;
}
void scene_manager::remove_instans( )
{
    delete _instans;
    _instans = nullptr;
}
scene_manager::scene_manager( )
{

}
scene_manager::~scene_manager( )
{
    delete _instans;
}
