#include "config_http.hpp"
//#include "config.hpp"
#include <iostream>
#include "global.hpp"
#include "utility.hpp"

ConfigHttp::ConfigHttp()
{
    ;
}

ConfigHttp::~ConfigHttp()
{
    DEBUG("ConfigHttp::~ConfigHttp() delete servers:" + Utility::to_string(this->servers.size()));
    for(size_t i=0;i<this->servers.size();i++){
        delete this->servers[i];
    }
}


ConfigServer const * ConfigHttp::server(size_t i) const
{
    if(i < this->get_server_size()){
        return (this->servers[i]);
    }else{
        ERROR("Invalid ConfigHttp Error: Invalid index");
        throw std::runtime_error("config error:http");
    }
    return (NULL);
}

size_t ConfigHttp::get_server_size() const
{
    return (this->servers.size());
}


void ConfigHttp::set_max_body_size(std::vector<std::string> &vec)
{
    size_t word_cnt = vec.size();
    if(word_cnt == 2)
    {
        this->client_max_body_size = BodySize::from_string(vec[1]);
    }else{
        ERROR("Invalid Config Error:invalid word is in server_name directive");
        throw std::runtime_error("config parser error:server");
    }
}

void ConfigHttp::assign_properties(std::vector<std::vector<std::string> > &properties)
{
    DEBUG("ConfigHttp::assign_properties()");
    std::vector<std::vector<std::string> >::iterator ite = properties.begin();
    std::vector<std::vector<std::string> >::iterator end = properties.end();
    while(ite != end){
        std::vector<std::string> &tmp_vec = *ite;
        for(size_t i=0;i<tmp_vec.size();i++){
            if(tmp_vec[0] == "client_max_body_size"){
                set_max_body_size(tmp_vec);
            }else{
                ERROR("Invalid Config Error:" + tmp_vec[0] + " is not server directive");
                throw std::runtime_error("config parser error:server");
            }
        }
        ite++;
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

size_t ConfigHttp::get_max_body_size()
{
    return (this->client_max_body_size.to_number());
}

void ConfigHttp::check()
{
    if(this->get_server_size() == 0)
    {
        ERROR("ConfigHttp::check(), servers size is 0");
        throw std::runtime_error("ConfigHttp::check(), servers size is 0");
    }
    if(this->get_server_size() >= 2){
        size_t default_server_count = 0;
        for(size_t i=0;i<this->get_server_size(); i++){
            if(this->server(i)->is_default_server()){
                default_server_count++;
            }

        }
        if(default_server_count >=2){
            ERROR("ConfigHttp::check(), default_servers count is >= 2");
            throw std::runtime_error("ConfigHttp::check(), default_servers count is >= 2");
        }
    }

}
