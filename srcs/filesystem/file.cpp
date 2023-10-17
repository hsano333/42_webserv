#include <sys/stat.h>
#include "global.hpp"
#include "file.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

File::File(IReader* ireader, std::string &filepath, RWOption option)
    :
        ireader(ireader),
        filepath(filepath),
        option(option)
{
    ;
}

File::File()
{
    ;
}

File::~File()
{

    std::cout << "close file:" << this->filepath << std::endl;
    std::cout << "fd:" << fd << std::endl;

    MYINFO("close file:" + this->filepath);
    if(this->fd > 0){
        std::cout << "fd:" << this->fd << std::endl;
        close(fd);
    }
}


File File::from_string(IReader* ireader, std::string &filepath, RWOption option)
{
    return (File(ireader, filepath, option));
}

void File::open_file()
{
    try
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
            MYINFO("open file:" + this->filepath + ",fd=" + Utility::to_string(this->fd));
            //this->fd = FileDiscriptor::from_int(tmp_fd);
        }
        else if (this->option == WRITE_ONLY)
        {
            this->fd = open(this->filepath.c_str(), O_WRONLY | O_CREAT | O_TRUNC);
            //this->fd = FileDiscriptor::from_int(tmp_fd);
        }else{
            ERROR("Invalid file option");
            throw std::invalid_argument("Invalid filepath option");
        }
        this->state = FILE_OPEN;
        if(this->fd < 0)
        {
            ERROR("cannot open file");
            throw std::runtime_error("can't  open file");
        }

    }catch(std::invalid_argument &e){
        ERROR("cannot open file");
        throw std::runtime_error("can't  open file");
    }
}

void File::close_file()
{
    if (this->state == FILE_OPEN){
        if(this->fd > 0){
            close(fd);
        }
    }
}

int File::read(char *buf)
{
    //(void)buf;
    //return 0;
    //
    
    //int tmp_fd = 3;
    //return ireader.iread(tmp_fd, buf);
    FileDiscriptor tmp_fd = FileDiscriptor::from_int(this->fd);
    return ireader->read(tmp_fd, buf, MAX_BUF);
}

/*
std::string File::to_str()
{
    return filepath;
}
*/
