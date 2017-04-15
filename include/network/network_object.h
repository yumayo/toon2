#pragma once
#include <string>
#include "use_destroy_object.h"
#include "utility/user_pointer.hpp"
namespace network
{
class network_object : public use_destroy_object
{
    // mutableはconst関数の中で値を書き換えたい場合に付けられる修飾子です。
    // http://soundengine.jp/wordpress/tips/tutorial/329/

    float mutable _second = 0.0F;
    float const _timeout_second = 60.0F;
public:
    network_object( ) = delete;
    network_object( std::string ip_address, int port );
    ~network_object( );
    void update( float delta_second ) const;
    void timeout_restart( ) const;
    network_object const& operator=( network_object const& other ) const;
    bool operator==( network_object const& other ) const;
public:
    std::string const ip_address;
    int const port;
};
using network_handle = utility::softptr<network_object>;
}
