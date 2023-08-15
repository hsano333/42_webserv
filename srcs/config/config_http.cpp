#include "config_http.hpp"
//#include "config.hpp"
#include <iostream>
#include "global.hpp"

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

void ConfigHttp::assign_properties(std::vector<std::vector<std::string> > &properties)
{
    if(properties.size() > 0){
        ERROR("Invalid Config HTTP Error: There is invalid properties");
        throw std::runtime_error("config parser error:http");
    }
}

void ConfigHttp::assign_out_properties(std::vector<std::string> &properties)
{
    if(properties.size() > 0){
        ERROR("Invalid Config HTTP Error: There is invalid word between \"http\" and \"{\" ");
        throw std::runtime_error("config parser error:http");
    }
}

void ConfigHttp::push_all(std::vector<ConfigServer*> const &vec)
{
    for(size_t i=0;i<vec.size();i++){
        this->servers.push_back(vec[i]);
    }
}

