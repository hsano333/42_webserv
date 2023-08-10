#include <sys/stat.h>
#include "global.hpp"
#include "filepath.hpp"

Filepath::Filepath(std::string filepath, RWOption option)
                    :filepath(filepath),
                    option(option)
{
    struct stat file_info;
    if (stat(filepath.c_str(), &file_info) != 0){
        ERROR("Invalid filepath:" + filepath);
        throw std::invalid_argument("Invalid filepath");
    }

    if(option == READ_ONLY){
        if(!(file_info.st_mode & 0000400)){
            ERROR("can't read file:" + filepath);
            throw std::runtime_error("can't read file");
        }
    }else if (option == WRITE_ONLY){
        if(!(file_info.st_mode & 0000200)){
            ERROR("can't write file:" + filepath);
            throw std::runtime_error("can't write file");
        }
    }else{
        ERROR("Invalid file option");
        throw std::invalid_argument("Invalid filepath option");
    }
}
Filepath::~Filepath()
{
    ;
}

std::string Filepath::to_str()
{
    return filepath;
}
