#include "player_manager_host.h"
#include "network/udp_connection.h"
#include "jsoncpp/json.h"
#include "utility/string_utility.h"
#include "utility/assert_log.h"
#include "boost/lexical_cast.hpp"
#include "action.hpp"
#include "user_default.h"
using namespace cinder;
namespace user
{
CREATE_CPP( player_manager_host, Json::Value& root, std::shared_ptr<network::udp_connection>& connection )
{
    CREATE( player_manager_host, root, connection );
}
bool player_manager_host::init( Json::Value& root, std::shared_ptr<network::udp_connection>& connection )
{
    if ( !player_manager::init( root, connection ) ) return false;

    // 一秒ごとにサーバーにホストを維持しているかの情報を送ります。
    // サーバーの方で一分間応答がないとタイムアウトになるようにしているので。
    std::string address = user_default::get_instans( )->get_root( )["server"]["address"].asString( );
    int port = user_default::get_instans( )->get_root( )["server"]["port"].asInt( );
    _udp.lock( )->run_action
    (
        action::repeat_forever::create
        (
            action::sequence::create
            (
                action::delay::create( 1.0F ),
                action::call_func::create
                (
                    [ this, address, port ] { _udp.lock( )->write( std::make_shared<network::network_object>( address, port ),
                                                                   "{\"name\":\"host_connection\"}" ); }
                )
            )
        )
    );

    return true;
}
}