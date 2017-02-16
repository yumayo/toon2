#pragma once
#include <vector>
#include <string>
namespace utility
{
// cocos2d-x���AUTF-8�̕�������ꕶ���ꕶ���ɕ�������֐��Q�B
// cocos2d/cocos/base/ccUTF8.* �Ȃǂ��Q�l�ɂ��܂����B
class utf8 : public std::vector<std::string>
{
    typedef unsigned char utf8_head;
    const char trailingBytesForUTF8[256] =
    {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
    };
    bool isLegalUTF8( const utf8_head *source, int length );
    int getUTF8StringLength( const utf8_head* utf8 );
    size_t getNumBytesForUTF8( utf8_head first );
    bool _is_utf8 = true;
    std::string _text;
public:
    utf8( );
    utf8( const std::string& newStr );
    bool is_utf8( );
    char const* c_str( );
    std::string string( );
};
}