#include "scene_manager.h"
#include "utility/assert_log.h"
scene_manager* scene_manager::_instans = nullptr;
void scene_manager::replace( std::shared_ptr<scene> const & scene )
{
    _fn.emplace_back( [ this, scene ] { _stack.pop( ); _stack.push( scene ); } );
}
void scene_manager::push( std::shared_ptr<scene> const & scene )
{
    _fn.emplace_back( [ this, scene ] { _stack.push( scene ); } );
}
void scene_manager::pop( )
{
    assert_log( 1 <= _stack.size( ), "これ以上ポップできません。", return );
    _fn.emplace_back( [ this ] { _stack.pop( ); } );
}
size_t scene_manager::size( )
{
    return _stack.size( );
}
bool scene_manager::empty( )
{
    return _stack.empty( );
}
std::shared_ptr<scene>& scene_manager::top( )
{
    return _stack.top( );
}
void scene_manager::update( )
{
    for ( auto& fn : _fn )
    {
        fn( );
    }
    _fn.clear( );
}
std::weak_ptr<node> scene_manager::get_dont_destroy_node( )
{
    return _root;
}
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
    _root = node::create( );
}
