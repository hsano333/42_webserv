#include "config_cgi.hpp"
#include "global.hpp"
#include "utility.hpp"
#include "split.hpp"
#include <iostream>

using std::cout;
using std::endl;

ConfigCgi::ConfigCgi()
{
    ;
}


/*
ConfigCgi::ConfigCgi(std::string &str)
{
    std::cout << str << std::endl;
}
*/

ConfigCgi::~ConfigCgi()
{
}

/*
void ConfigCgi::parse()
{
}
*/


std::map<std::string, std::string> &ConfigCgi::get_extensions()
{
    return (this->extensions);
}

void ConfigCgi::assign_properties(std::vector<std::vector<std::string> > &properties)
{
    std::vector<std::vector<std::string> >::iterator ite = properties.begin();
    std::vector<std::vector<std::string> >::iterator end = properties.end();
    while(ite != end){
        std::vector<std::string> &tmp_vec = *ite;
        std::string line = tmp_vec[0];
        for(size_t i=1;i<tmp_vec.size();i++){
            line += " ";
            line += tmp_vec[i];
        }
        Split sp(line, ":", false, true);
        if (sp.size() == 2){
            this->extensions.insert(std::make_pair(
                        Utility::trim_white_space(sp[0]),
                        Utility::trim_white_space(sp[1])
                    ));
        }
        ite++;
    }
}

void ConfigCgi::assign_out_properties(std::vector<std::string> &properties)
{
    (void)properties;
    WARNING("ConfigCgi::assign_out_properties() does not used");
    /*
    for(size_t i=0;i<properties.size();i++){
        this->allowed_method_.push_back(Method::from_string(properties[i]));
    }
    */
}

void ConfigCgi::push_all(std::vector<ConfigLastObject*> const &vec)
{
    (void)vec;
    WARNING("ConfigCgi::push_all() does not used");
}



void ConfigCgi::check()
{
    std::map<std::string, std::string>::iterator ite = this->extensions.begin();
    std::map<std::string, std::string>::iterator end = this->extensions.end();
    while(ite != end){
        std::string ext = ite->first;
        std::string &path = ite->second;
        cout << "ext:" << ext << endl;
        cout << "path:" << path << endl;
        if(ext[0] != '.'){
            ERROR("ConfigCgi::check() [.]dot is necessary at the begining:" + ext);
            throw std::runtime_error("ConfigCgi::check() [.]dot is necessary at the begining");
        }
        if (Utility::is_executable_file(path) == false){
            ERROR("Invalid Config Error: file is not regular file or not executable: " + path);
            throw std::runtime_error("Invalid Config Error: file is not regular file or not executable");
        }
        //if (stat(path.c_str(), &fileInfo) != 0)

        ite++;
    }
}
