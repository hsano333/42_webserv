#include "config_server.hpp"
#include <iostream>

ConfigServer::ConfigServer()
{
    ;
}


ConfigServer::ConfigServer(std::string &str)
{
    std::cout << str << std::endl;
}

ConfigServer::~ConfigServer()
{
}

/*
void ConfigServer::parse()
{
}
*/

void ConfigServer::add_properties(std::string &key, std::string &value)
{
    (void)key;
    (void)value;
}

ConfigLocation ConfigServer::get_next_object()
{
    ConfigLocation tmp;
    return tmp;
}
