#pragma once
#include "../node.h"
namespace network
{
class udp_sender : public node
{
    struct _member;
    std::shared_ptr<_member> _m;
public:
    CREATE_H( udp_sender, std::string const& ip_address, std::string const& port );
    bool init( std::string const& ip_address, std::string const& port );
public:
    void write( std::string const& message, std::function<void( )> on_send = nullptr );
    void write( char const* message, size_t size, std::function<void( )> on_send = nullptr );
    void close( );
public:
    // �f�[�^�𑗂�Ȃ������Ƃ��ɌĂ΂�܂��B
    std::function<void( )> on_send_failed;
    // �\�P�b�g�������Ƃ��ɌĂ΂�܂��B
    std::function<void( )> on_closed;
};
}