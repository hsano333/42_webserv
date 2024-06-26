#include "config_raw_loader.hpp"
#include "global.hpp"
#include "utility.hpp"
#include <iostream>

ConfigRawLoader::ConfigRawLoader(NormalFile *file)
                                :
                                file(file)
{
    ;
}

ConfigRawLoader::~ConfigRawLoader()
{
    delete file;
}

std::string ConfigRawLoader::get_raw_data()
{
    std::string all_data;
    this->file->open();
    char buf[MAX_BUF+1];
    char *tmp = &(buf[0]);
    int size = this->file->read(&tmp, MAX_BUF);

    while(size > 0){
        buf[size] = '\0';
        all_data += buf;
        size = file->read(&(tmp), MAX_READ_SIZE);
        if(size >= MAX_READ_SIZE){
            ERROR("Config file is bigger than:" + Utility::to_string(MAX_READ_SIZE));
            throw std::runtime_error("Config file is bigger than readable size");
        }
    }
    file->close();
    return all_data;
}
