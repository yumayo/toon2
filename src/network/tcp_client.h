#pragma once
#include "../node.h"
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
    // �n���h�V�F�C�N�Ɏ��s�����Ƃ��ɌĂ΂�܂��B
    std::function<void( )> on_handshake_faild;
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
public:
    LUA_SETUP_H( tcp_client );
private:
    void lua_write_string_default( std::string const& message );
    void lua_write_binary_default( char const* message, size_t size );
    void lua_write_string( std::string const& message, std::function<void( )> on_send );
    void lua_write_binary( char const* message, size_t size, std::function<void( )> on_send );
    std::function<void( int )> lua_on_errored;
};
}