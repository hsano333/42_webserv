#include <sys/stat.h>
#include "global.hpp"
#include "file.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

File::File(IReader& ireader, std::string &filepath, RWOption option)
    :
        ireader(ireader),
        filepath(filepath),
        option(option)
{
    ;
}
File::~File()
{
    if(fd >= 0){
        close(fd);
    }
}

void File::open_file()
{
    struct stat file_info;
    //char *filepath = this->filepath;
    if(this->option == READ_ONLY)
    {
        if (stat(this->filepath.c_str(), &file_info) != 0){
            ERROR("Invalid filepath:" + this->filepath);
            throw std::invalid_argument("Invalid filepath");
        }
        if(!(file_info.st_mode & S_IFREG)){
            ERROR("file is not regular file:" + this->filepath);
            throw std::invalid_argument("file is not regular file");
        }

        if(!(file_info.st_mode & 0000400)){
            ERROR("can't read file:" + this->filepath);
            throw std::runtime_error("can't read file");
        }
        this->fd = open(this->filepath.c_str(), O_RDONLY);
    }
    else if (this->option == WRITE_ONLY)
    {
        this->fd = open(this->filepath.c_str(), O_WRONLY | O_CREAT | O_TRUNC);
    }else{
        ERROR("Invalid file option");
        throw std::invalid_argument("Invalid filepath option");
    }
    if(this->fd < 0){
        ERROR("cannot open file");
        throw std::runtime_error("can't  open file");

    }
    this->state = FILE_OPEN;
}

void File::close_file()
{
    if (this->fd > 0 && this->state == FILE_OPEN){
        close(this->fd);
        this->fd = -1;
    }
}

int File::read(char *buf)
{
    return ireader.iread(this->fd, buf);
}

/*
std::string File::to_str()
{
    return filepath;
}
*/
