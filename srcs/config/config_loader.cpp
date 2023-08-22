#include "config_loader.hpp"


ConfigLoader::ConfigLoader()
{
    ;
}

ConfigLoader::~ConfigLoader()
{
    ;
}

ConfigLoader::load_file(std::string &filepath)
{
    ReadRaw read_raw;
    File file = File(read_raw);
    file.open_file(tmp_path, WRITE_ONLY);

    char buf[MAX_BUF];
    std::string 
    while(file.read(buf)){

    }
}
