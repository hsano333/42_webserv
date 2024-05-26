#include "config_limit.hpp"
#include "global.hpp"
#include <iostream>

ConfigLimit::ConfigLimit() : cgi_(NULL)
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
    //for(size_t i=0;i<this->cgis.size();i++){
    delete this->cgi_;
    //}
}

/*
void ConfigLimit::parse()
{
}
*/
const ConfigCgi *ConfigLimit::cgi() const
{
    return (this->cgi_);
}

/*
size_t ConfigLimit::get_cgi_size() const
{
    return (this->cgis.size());
}
*/

void ConfigLimit::set_cidr(std::vector<std::string> &vec, bool flag)
{
    size_t word_cnt = vec.size();
    if(word_cnt != 2)
    {
        ERROR("Invalid Config Error: limit_except deny/all directive is invalid");
        throw std::runtime_error("config parser error:limit [deny/all]");
    }
    CIDR cidr = CIDR::from_string(vec[1]);
    allowd_cidr.push_back(std::make_pair(cidr, flag));
}

void ConfigLimit::assign_properties(std::vector<std::vector<std::string> > &properties)
{
    DEBUG("ConfigLimit::assign_properties()");
    std::vector<std::vector<std::string> >::iterator ite = properties.begin();
    std::vector<std::vector<std::string> >::iterator end = properties.end();
    while(ite != end){
        std::vector<std::string> &tmp_vec = *ite;
        if(tmp_vec[0] == "deny"){
            set_cidr(tmp_vec, false);
        }else if(tmp_vec[0] == "allow"){
            set_cidr(tmp_vec, true);
        }else{
            ERROR("Invalid Config Error:" + tmp_vec[0] + " is not limit_except directive");
            throw std::runtime_error("config parser error:limit");
        }
        ite++;
    }
}

void ConfigLimit::assign_out_properties(std::vector<std::string> &properties)
{
    for(size_t i=0;i<properties.size();i++){
        this->allowed_method_.push_back(Method::from_string(properties[i]));
    }
}

void ConfigLimit::push_all(std::vector<ConfigCgi*> const &vec)
{
    if (vec.size() > 1){
        ERROR("ConfigLimit::push_all()  cgi directive is not one. delete unnecessary  cgi directive or add");
        throw std::runtime_error("ConfigLimit::push_all()  cgi directive is not one");
    }
    if (vec.size() == 0){
        this->cgi_ = NULL;
        return;
    }
    this->cgi_ = vec[0];
}

std::vector<Method> const & ConfigLimit::allowed_method() const
{
    return (allowed_method_);
}

std::string ConfigLimit::allowed_method_str() const
{

    if(allowed_method_.size() == 0){
        return "";
    }

    std::string method_string = allowed_method_[0].to_string();
    for(size_t i=1;i<allowed_method_.size();i++){
        method_string += " ";
        method_string += allowed_method_[i].to_string();
    }
    return (method_string);
}

std::vector<std::pair<CIDR, bool> > const & ConfigLimit::allowed_cidr() const
{
    return (allowd_cidr);
}

void ConfigLimit::check()
{
    //nothing to do;
    ;
}
