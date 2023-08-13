#include "config_raw_loader.hpp"
#include "global.hpp"
#include "utility.hpp"
#include <iostream>

ConfigRawLoader::ConfigRawLoader(File &file)
                                :
                                file(file)
{
    ;
}

ConfigRawLoader::~ConfigRawLoader()
{
    ;
}

std::string ConfigRawLoader::get_raw_data()
{
    std::string all_data;
    file.open_file();
    char buf[MAX_BUF];
    int size = file.read(buf);

    while(size > 0){
        buf[size] = '\0';
        all_data += buf;
        size = file.read(buf);
        if(size >= MAX_READ_SIZE){
            ERROR("Config file is bigger than:" + Utility::to_string(MAX_READ_SIZE));
            throw std::runtime_error("Config file is bigger than readable size");
        }
    }
    file.close_file();
    return all_data;
}
