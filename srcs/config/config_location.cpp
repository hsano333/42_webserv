#include "config_location.hpp"
#include "global.hpp"
#include "utility.hpp"
#include <iostream>

ConfigLocation::ConfigLocation() : root_(""), cgi_pass_(""), limit_(NULL), is_redirect_(false)
{
    ;
}

ConfigLocation::~ConfigLocation()
{
    DEBUG("ConfigLocation::~ConfigLocation delete limit:");
    delete this->limit_;
}

const ConfigLimit *ConfigLocation::limit() const
{
    return (this->limit_);
}

size_t ConfigLocation::get_limit_size() const
{
    return (1);
}


void ConfigLocation::assign_properties(std::vector<std::vector<std::string> > &properties)
{
    DEBUG("ConfigLocation::assign_properties()");
    std::vector<std::vector<std::string> >::iterator ite = properties.begin();
    std::vector<std::vector<std::string> >::iterator end = properties.end();
    while(ite != end){
        std::vector<std::string> &tmp_vec = *ite;
        if(tmp_vec[0] == "root"){
            set_root(tmp_vec);
        }else if(tmp_vec[0] == "cgi_pass"){
            set_cgi_pass(tmp_vec);
        }else if(tmp_vec[0] == "autoindex"){
            set_autoindex(tmp_vec);
        }else if(tmp_vec[0] == "index"){
            set_index(tmp_vec);
        }else if(tmp_vec[0] == "return"){
            set_return(tmp_vec);
        }else if(tmp_vec[0] == "error_page"){
            set_error_page(tmp_vec);
        }else{
            ERROR("Invalid Config Error:" + tmp_vec[0] + " is not location directive");
            throw std::runtime_error("config parser error:location");
        }
        ite++;
    }
}

void ConfigLocation::assign_out_properties(std::vector<std::string> &properties)
{
    for(size_t i=0;i<properties.size();i++){
        pathes_.push_back(Utility::remove_obstruction_in_uri(properties[i]));
    }
}

void ConfigLocation::push_all(std::vector<ConfigLimit*> const &vec)
{
    if (vec.size() != 1){
        ERROR("ConfigLocation::push_all() limit_except directive is not one. delete unnecessary limit_except directive or add");
        throw std::runtime_error("ConfigLocation::push_all() limit_except directive is more than one");

    }
    this->limit_ = vec[0];
}



std::vector<std::string> const & ConfigLocation::pathes() const
{
    return ((this->pathes_));
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

/*
std::map<StatusCode, std::string> const & ConfigLocation::error_pages() const
{
    return (this->error_pages_);
}
*/


std::vector<std::string> const & ConfigLocation::indexes() const
{
    return (this->indexes_);
}

bool ConfigLocation::is_redirect() const
{
    return (this->is_redirect_);
}

const std::pair<StatusCode, std::string> &ConfigLocation::redirect() const
{
    return (this->redirect_);
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
    this->root_ = Utility::remove_obstruction_in_uri(vec[1]);
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
    this->cgi_pass_ = Utility::remove_obstruction_in_uri(vec[1]);
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
    }else if(vec[1] == "off"){
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
        this->indexes_.push_back(Utility::remove_obstruction_in_uri(vec[i]));
    }
}

void ConfigLocation::set_return(std::vector<std::string> &vec)
{
    if (this->is_redirect_){
        ERROR("Redirect directive is multipled");
        throw std::runtime_error("Redirect directive is multipled");
    }

    size_t word_cnt = vec.size();
    if(word_cnt == 3)
    {
        StatusCode status_code = StatusCode::from_string(vec[1]);

        if(status_code < 300 || status_code >= 400){
            ERROR("Redirect directive is invalid code:" + status_code.to_string());
            throw std::runtime_error("Redirect code is invalid");
        }

        this->redirect_ = std::make_pair(status_code, Utility::remove_obstruction_in_uri(vec[2]));
        this->is_redirect_ = true;
    }else{
        this->is_redirect_ = false;
        ERROR("Redirect(return) directive is Invalid");
        throw std::runtime_error("Redirect(return) directive is Invalid :" + vec[0]);
    }
}

std::map<StatusCode, std::string> const &ConfigLocation::error_pages() const
{
    return (this->error_pages_);
}

std::string ConfigLocation::get_error_file_path(StatusCode &code) const
{
    if(this->error_pages_.find(code) == this->error_pages_.end()){
        throw std::runtime_error("Not Found");
    }
    return (root_ + this->error_pages_.at(code));
}

void ConfigLocation::set_error_page(std::vector<std::string> &vec)
{
    size_t word_cnt = vec.size();
    if(word_cnt <= 1)
    {
        ERROR("Invalid Config Error: error_page directive is invalid");
        throw std::runtime_error("config parser error:server [error_page]");
    }
    std::string path = Utility::remove_obstruction_in_uri(vec[vec.size()-1]);
    for(size_t i=1;i<vec.size()-1;i++){
        StatusCode status_code = StatusCode::from_string(vec[i]);
        this->error_pages_.insert(std::make_pair(status_code, path));
    }
}


/*
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
*/


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

    if (this->indexes_.size() > 1){
        ERROR("ConfigLocation::check(), index file is duplicated");
        throw std::runtime_error("ConfigLocation::check(), index file is duplicated");
    }
}

bool ConfigLocation::is_allowed_method(Method method) const
{
    DEBUG("ConfigLocation::is_allowed_method:method=" + method.to_string());
    const ConfigLimit *limit = this->limit();
    std::vector<Method>::const_iterator begin = limit->allowed_method().begin();
    std::vector<Method>::const_iterator end = limit->allowed_method().end();

    while(begin != end){
        if(method == *begin){
        DEBUG("ConfigLocation::is_allowed_method: No.1 method=" + method.to_string());
            return (true);
        }
        begin++;
    }
        DEBUG("ConfigLocation::is_allowed_method: No.2 method=" + method.to_string());
    return (false);
}
