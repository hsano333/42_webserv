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

void ConfigLocation::assign_properties(std::vector<std::vector<std::string> > &properties)
{
    (void)properties;
}

void ConfigLocation::assign_out_properties(std::vector<std::string> &properties)
{
    for(size_t i=0;i<properties.size();i++){
        out_properties.push_back(properties[i]);
    }
}

void ConfigLocation::push_all(std::vector<ConfigLimit*> const &vec)
{
    for(size_t i=0;i<vec.size();i++){
        this->limits.push_back(vec[i]);
    }
}

