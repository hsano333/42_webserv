#include "config_repository.hpp"
#include <iostream>

using std::cout;
using std::endl;

ConfigRepository::ConfigRepository(File file) : 
    cfg_file(file)
{
    ;
}

ConfigRepository::~ConfigRepository()
{
    ;
}

void ConfigRepository::load_file(std::string &filepath)
{
    cout << "Config Repository load89" << endl;
}

void ConfigRepository::set_data(std::string &data)
{
    this->config_raw_data = data;
}


