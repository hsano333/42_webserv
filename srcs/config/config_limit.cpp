#include "config_limit.hpp"
#include "global.hpp"
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

void ConfigLimit::assign_properties(std::vector<std::vector<std::string> > &properties)
{
    (void)properties;
}

void ConfigLimit::assign_out_properties(std::vector<std::string> &properties)
{
    for(size_t i=0;i<properties.size();i++){
        out_properties.push_back(properties[i]);
    }
}

void ConfigLimit::push_all(std::vector<ConfigLastObject*> const &vec)
{
    if(vec.size() != 0){
        ERROR("Invalid Config Error: don't push anything in ConfigLimit ");
        throw std::runtime_error("config parser error:limit");
    }
}
