#include "network_factory.h"
#include "utility/string_utility.h"
namespace network
{
network_factory::network_factory( udp_connection & connection )
    : _connection( connection )
{
}
network_handle network_factory::regist( std::string const & ip_address, int const & port )
{
    utility::scoped_mutex mutex( _connection.get_mutex( ) );

    auto regist_object = std::make_shared<network_object>( ip_address, port );
    auto itr = find_network_object( regist_object );
    if ( itr == std::end( _network_objects ) )
    {
        _network_objects.emplace_back( regist_object );
        return regist_object;
    }
    else
    {
        return ( *itr );
    }
}
std::list<std::shared_ptr<network_object>>::iterator network_factory::find_network_object( network_handle handle )
{
    utility::scoped_mutex mutex( _connection.get_mutex( ) );

    auto itr = std::find_if( std::begin( _network_objects ), std::end( _network_objects ),
                             [ handle ] ( std::shared_ptr<network_object>& object )
    {
        return *object == **handle;
    } );
    if ( itr != std::end( _network_objects ) )
    {
        return itr;
    }
    return std::end( _network_objects );
}
void network_factory::update( float delta_second )
{
    utility::scoped_mutex mutex( _connection.get_mutex( ) );

    auto remove_itr = std::remove_if( std::begin( _network_objects ), std::end( _network_objects ),
                                      [ delta_second ] ( std::shared_ptr<network_object>& objects )
    {
        objects->update( delta_second );
        if ( objects->is_destroy( ) )
        {
            utility::log_network( objects->ip_address, objects->port,
                                  "タイムアウトになりました。このオブジェクトは削除されました。" );
            return true;
        }
        return false;
    } );
    _network_objects.erase( remove_itr, std::end( _network_objects ) );
}
}
