#include "config_location.hpp"
#include "global.hpp"
#include "utility.hpp"
#include <iostream>

ConfigLocation::ConfigLocation() : root_(""), cgi_pass_("")
{
    ;
}

ConfigLocation::~ConfigLocation()
{
    DEBUG("ConfigLocation::~ConfigLocation delete limits:" + Utility::to_string(this->limits.size()));
    for(size_t i=0;i<this->limits.size();i++){
        delete this->limits[i];
    }
}

ConfigLimit const *ConfigLocation::limit(size_t i) const
{
    if(i < this->get_limit_size()){
        return (this->limits[i]);
    }
    ERROR("Invalid ConfigLocation Error:Invalid index ");
    throw std::runtime_error("config error:location");
}

size_t ConfigLocation::get_limit_size() const
{
    return (this->limits.size());
}


void ConfigLocation::assign_properties(std::vector<std::vector<std::string> > &properties)
{
    std::vector<std::vector<std::string> >::iterator ite = properties.begin();
    std::vector<std::vector<std::string> >::iterator end = properties.end();
    while(ite != end){
        std::vector<std::string> &tmp_vec = *ite;
        for(size_t i=0;i<tmp_vec.size();i++){
            if(tmp_vec[0] == "root"){
                set_root(tmp_vec);
            }else if(tmp_vec[0] == "cgi_pass"){
                set_cgi_pass(tmp_vec);
            }else if(tmp_vec[0] == "autoindex"){
                set_autoindex(tmp_vec);
            }else if(tmp_vec[0] == "index"){
                set_index(tmp_vec);
            }else if(tmp_vec[0] == "error_page"){
                set_error_page(tmp_vec);
            //}else if(tmp_vec[0] == "cgi"){
                //std::cout << "cgi" << std::endl;
                //for(size_t i=0;i<tmp_vec.size();i++){
                    //std::cout << tmp_vec[i] << std::endl;
                //}

            }else{
                ERROR("Invalid Config Error:" + tmp_vec[0] + " is not location directive");
                throw std::runtime_error("config parser error:location");
            }
        }
        ite++;
    }
}

void ConfigLocation::assign_out_properties(std::vector<std::string> &properties)
{
    for(size_t i=0;i<properties.size();i++){
        pathes_.push_back(properties[i]);
    }
}

void ConfigLocation::push_all(std::vector<ConfigLimit*> const &vec)
{
    for(size_t i=0;i<vec.size();i++){
        this->limits.push_back(vec[i]);
    }
}



std::vector<std::string> const & ConfigLocation::pathes() const
{
    return (this->pathes_);
}

std::string const & ConfigLocation::root() const
{
    return (this->root_);
}


std::string const & ConfigLocation::cgi_pass() const
{
    return (this->cgi_pass_);
}

bool ConfigLocation::autoindex() const
{
    return (this->autoindex_);
}

std::map<StatusCode, std::string> const & ConfigLocation::error_pages() const
{
    return (this->error_pages_);
}


std::vector<std::string> const & ConfigLocation::indexes() const
{
    return (this->indexes_);
}

void ConfigLocation::set_root(std::vector<std::string> &vec)
{
    size_t word_cnt = vec.size();
    if(word_cnt != 2)
    {
        ERROR("Invalid Config Error: root directive is invalid");
        throw std::runtime_error("config parser error:location [root]");
    }
    if (this->cgi_pass_ != "")
    {
        ERROR("Invalid Config Location Error: you cannot set both [root] and [cgi_pass]");
        throw std::runtime_error("Invalid Config Location Error: you cannot set both [root] and [cgi_pass]");
    }
    this->root_ = vec[1];
}

void ConfigLocation::set_cgi_pass(std::vector<std::string> &vec)
{
    size_t word_cnt = vec.size();
    if(word_cnt != 2)
    {
        ERROR("Invalid Config Error: cgi_pass directive is invalid");
        throw std::runtime_error("config parser error:location [cgi_pass]");
    }
    if (this->root_ != "")
    {
        ERROR("Invalid Config Location Error: you cannot set both [root] and [cgi_pass]");
        throw std::runtime_error("Invalid Config Location Error: you cannot set both [root] and [cgi_pass]");
    }
    this->cgi_pass_ = vec[1];
}

void ConfigLocation::set_autoindex(std::vector<std::string> &vec)
{
    size_t word_cnt = vec.size();
    if(word_cnt != 2)
    {
        ERROR("Invalid Config Error: autoindex directive is invalid");
        throw std::runtime_error("config parser error:location [autoindex]");
    }
    if(vec[1] == "on"){
        this->autoindex_ = true;
    }else if(vec[1] == "on"){
        this->autoindex_ = false;
    }else{
        ERROR("Invalid Config Error: autoindex directive is invalid");
        throw std::runtime_error("config parser error:location [autoindex]");
    }
}

void ConfigLocation::set_index(std::vector<std::string> &vec)
{
    size_t word_cnt = vec.size();
    if(word_cnt <= 1)
    {
        ERROR("Invalid Config Error: index directive is invalid");
        throw std::runtime_error("config parser error:location [index]");
    }
    for(size_t i=1;i<vec.size();i++){
        this->indexes_.push_back(vec[i]);
    }
}

void ConfigLocation::set_error_page(std::vector<std::string> &vec)
{
    size_t word_cnt = vec.size();
    if(word_cnt <= 1)
    {
        ERROR("Invalid Config Error: error_page directive is invalid");
        throw std::runtime_error("config parser error:location [error_page]");
    }
    std::string path = vec[vec.size()-1];
    for(size_t i=1;i<vec.size()-1;i++){
        StatusCode status_code = StatusCode::from_string(vec[i]);
        this->error_pages_.insert(std::make_pair(status_code, path));
    }
}


void ConfigLocation::check()
{
    if(pathes_.size() == 0)
    {
        ERROR("ConfigLocation::check(), pathes_ size is 0");
        throw std::runtime_error("ConfigLocation::check(), pathes_ size is 0");
    }
    else if(root_ == "" && cgi_pass_ == "")
    {
        ERROR("ConfigLocation::check(), Neither root_ nor cgi_pass is set");
        throw std::runtime_error("ConfigLocation::check(), Neither root_ nor cgi_pass is set");
    }
}
