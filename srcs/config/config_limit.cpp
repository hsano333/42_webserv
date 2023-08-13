#include "config_limit.hpp"
#include <iostream>

ConfigLimit::ConfigLimit()
{
    ;
}


/*
ConfigLimit::ConfigLimit(std::string &str)
{
    std::cout << str << std::endl;
}
*/

ConfigLimit::~ConfigLimit()
{
}

/*
void ConfigLimit::parse()
{
}
*/

void ConfigLimit::add_properties(std::string &key, std::string &value)
{
    (void)key;
    (void)value;
}

ConfigLastObject ConfigLimit::get_next_object()
{
    ConfigLastObject tmp;
    return tmp;
}
