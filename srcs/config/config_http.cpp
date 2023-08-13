#include "config_http.hpp"
//#include "config.hpp"
#include <iostream>

ConfigHttp::ConfigHttp()
{
    ;
}
ConfigHttp::ConfigHttp(std::string &str)
{
    std::cout << str << std::endl;
}

ConfigHttp::~ConfigHttp()
{
}

void ConfigHttp::parse()
{
}

void ConfigHttp::add_properties(std::string &key, std::string &value)
{
    (void)key;
    (void)value;
}

ConfigServer ConfigHttp::get_next_object()
{
    //std::string tmp = "";
    ConfigServer tmp;
    return tmp;
}
