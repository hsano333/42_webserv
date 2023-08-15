#include "config_server.hpp"
#include <iostream>
#include "global.hpp"

ConfigServer::ConfigServer() : is_default_server(false)
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


void ConfigServer::set_listen(std::vector<std::string> &vec)
{
    size_t word_cnt = vec.size();

    /*
    for(size_t i=0;i<vec.size();i++){
        std::cout << vec[i] << std::endl;
    }
    */


    if(word_cnt >= 2 && word_cnt <= 3){
        this->listen = Port::from_string(vec[1]);
        if(word_cnt == 3){
            if(vec[2] == "default_server"){
                this->is_default_server = true;
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
        this->server_name = vec[1];
    }else{
        ERROR("Invalid Config Error:invalid word is in server_name directive");
        throw std::runtime_error("config parser error:server");
    }
}

void ConfigServer::assign_properties(std::vector<std::vector<std::string> > &properties)
{

    std::vector<std::vector<std::string> >::iterator ite = properties.begin();
    std::vector<std::vector<std::string> >::iterator end = properties.end();
    while(ite != end){
        std::vector<std::string> &tmp_vec = *ite;
        for(size_t i=0;i<tmp_vec.size();i++){
            if(tmp_vec[0] == "listen"){
                set_listen(tmp_vec);
            }else if(tmp_vec[0] == "server_name"){
                set_server_name(tmp_vec);
            }else{
                ERROR("Invalid Config Error:" + tmp_vec[0] + " is not server directive");
                throw std::runtime_error("config parser error:server");
            }
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
