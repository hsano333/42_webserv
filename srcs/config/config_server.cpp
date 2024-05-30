#include "config_server.hpp"
#include "ip_address.hpp"
#include <iostream>
#include "global.hpp"
#include "utility.hpp"

using std::cout;
using std::endl;

ConfigServer::ConfigServer() : is_default_server_(false)
{
    ;
}

ConfigServer::~ConfigServer()
{
    DEBUG("ConfigServer::~ConfigServer() delete locations:" + Utility::to_string(this->locations.size()));
    for(size_t i=0;i<this->locations.size();i++){
        delete this->locations[i];
    }
}

ConfigLocation const * ConfigServer::location(size_t i) const
{
    if(i < this->get_location_size()){
        return (this->locations[i]);
    }
    ERROR("Invalid ConfigServer Error:Invalid index ");
    throw std::runtime_error("config error:server");
}

size_t ConfigServer::get_location_size() const
{
    return (this->locations.size());
}


std::string const & ConfigServer::server_name() const
{
    return (this->server_name_);
}

IP_Address const & ConfigServer::server_addr() const
{
    return (this->server_addr_);
}

Port const & ConfigServer::listen() const
{
    return (this->listen_);
}

bool ConfigServer::is_default_server() const
{
    return (this->is_default_server_);
}



size_t ConfigServer::get_max_body_size() const
{
    return (this->client_max_body_size.to_number());
}


void ConfigServer::set_listen(std::vector<std::string> &vec)
{
    size_t word_cnt = vec.size();

    if(word_cnt >= 2 && word_cnt <= 3){
        this->listen_ = Port::from_string(vec[1]);
        if(word_cnt == 3){
            if(vec[2] == "default_server"){
                this->is_default_server_ = true;
            }else{
                ERROR("Invalid Config Error:" + vec[2] + " is not server directive");
                throw std::runtime_error("config parser error:server");
            }
        }
    }
}


void ConfigServer::set_server_name(std::vector<std::string> &vec)
{
    size_t word_cnt = vec.size();
    if(word_cnt == 2)
    {
        this->server_name_ = vec[1];
        this->server_addr_ = IP_Address::from_string_or_name(vec[1]);
    }else{
        ERROR("Invalid Config Error:invalid word is in server_name directive");
        throw std::runtime_error("config parser error:server");
    }
}


void ConfigServer::assign_properties(std::vector<std::vector<std::string> > &properties)
{
    DEBUG("ConfigServer::assign_properties()");
    std::vector<std::vector<std::string> >::iterator ite = properties.begin();
    std::vector<std::vector<std::string> >::iterator end = properties.end();
    while(ite != end){
        std::vector<std::string> &tmp_vec = *ite;
        cout << "tmp_vec = " << tmp_vec[0] << endl;
        if(tmp_vec[0] == "listen"){
            set_listen(tmp_vec);
        }else if(tmp_vec[0] == "server_names"){
            set_server_name(tmp_vec);
        }else if(tmp_vec[0] == "client_max_body_size"){
            set_max_body_size(tmp_vec);
        }else{
            ERROR("Invalid Config Error:" + tmp_vec[0] + " is not server directive");
            throw std::runtime_error("config parser error:server");
        }
        ite++;
    }
    (void)properties;

}

void ConfigServer::assign_out_properties(std::vector<std::string> &properties)
{
    if(properties.size() > 0){
        std::cout << "NG:" << properties.size() << std::endl;
        std::cout << "NG:[" << properties[0] << "]" << std::endl;
        ERROR("Invalid Config Error: There is invalid word between \"server\" and \"{\" ");
        throw std::runtime_error("config parser error:server");
    }
}

void ConfigServer::push_all(std::vector<ConfigLocation*> const &vec)
{
    for(size_t i=0;i<vec.size();i++){
        this->locations.push_back(vec[i]);
    }
}

void ConfigServer::check()
{
    if(this->get_location_size() == 0)
    {
        ERROR("ConfigServer::check(), locations size is 0");
        throw std::runtime_error("ConfigServer::check(), locations size is 0");
    }
    if(!this->listen_.is_valid()){
        ERROR("ConfigServer::check(),  Port is invalid");
        throw std::runtime_error("ConfigServer::check(),  Port is invalid");
    }
    //if(this->server_name() == ""){

    //}
    //else if()

}


void ConfigServer::set_max_body_size(std::vector<std::string> &vec)
{
    size_t word_cnt = vec.size();
    if(word_cnt == 2)
    {
        this->client_max_body_size = BodySize::from_string(vec[1]);
    }else{
        ERROR("Invalid Config Server Error:invalid word is in server directive");
        throw std::runtime_error("config parser error:server");
    }
}

