#pragma once
#include "jsoncpp/json.h"
class user_default
{
public:
    Json::Value& get_root( );
    void save( );
private:
    Json::Value _root;
    std::string _target_file_name = "user_default.json";
public:
    static user_default* get_instans( );
    static void remove_instans( );
private:
    user_default( );
    static user_default* _instans;
};