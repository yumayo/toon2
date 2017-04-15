#pragma once
#include "node.h"
#include "asio/error.hpp"
namespace network
{
class tcp_client : public node
{
    struct _member;
    std::shared_ptr<_member> _m;
public:
    CREATE_H( tcp_client, std::string const& ip_address, std::string const& port );
    bool init( std::string const& ip_address, std::string const& port );
    ~tcp_client( );
    void update( float delta ) override;
public:
    void write( std::string const& message, std::function<void( )> on_send = nullptr );
    void write( char const* message, size_t size, std::function<void( )> on_send = nullptr );
public:
    // �f�[�^�𑗂�Ȃ������Ƃ��ɌĂ΂�܂��B
    std::function<void( )> on_send_failed;
    // �f�[�^�������Ă����Ƃ��ɌĂ΂�܂��B
    std::function<void( char const*, size_t )> on_readed;
    // �ڑ��ł��Ȃ������Ƃ��ɌĂ΂�܂��B
    std::function<void( )> on_connect_failed;
    // �ڑ����؂ꂽ�Ƃ��ɌĂ΂�܂��B
    std::function<void( )> on_disconnected;
    // �\�P�b�g�������Ƃ��ɌĂ΂�܂��B
    std::function<void( )> on_closed;
    // ���̑��̃G���[���o���Ƃ��ɌĂ΂�܂��B
    std::function<void( asio::error_code const& )> on_errored;
};
}