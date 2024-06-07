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
    ReadNormal read_normal;
    File file = File(read_normal);
    file.open_file(tmp_path, WRITE_ONLY);
}
