#pragma once
#include <string>
class write_file_on_destroyed : public std::string
{
    std::string _filename;
public:
    write_file_on_destroyed( ) = delete;
    write_file_on_destroyed( std::string const& filename );
    ~write_file_on_destroyed( );
    void operator()( char const* str, ... );
};