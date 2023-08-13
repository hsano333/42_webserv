#include "config_location.hpp"
#include <iostream>

ConfigLocation::ConfigLocation()
{
    ;
}


ConfigLocation::ConfigLocation(std::string &str)
{
    std::cout << str << std::endl;
}

ConfigLocation::~ConfigLocation()
{
}

/*
void ConfigLocation::parse()
{
}
*/

void ConfigLocation::add_properties(std::string &key, std::string &value)
{
    (void)key;
    (void)value;
}

ConfigLimit ConfigLocation::get_next_object()
{
    ConfigLimit tmp;
    return tmp;
}
