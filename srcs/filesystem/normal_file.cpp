#include <sys/stat.h>
#include "global.hpp"
#include "normal_file.hpp"
#include "normal_reader.hpp"
#include "stream_reader.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

using std::cout;
using std::endl;

NormalFile::NormalFile(IReader* ireader, std::string const &filepath, std::ios_base::openmode option)
    :
        ireader(ireader),
        filepath(filepath),
        option(option)
        //ifs(std::ifstream(filepath.c_str(), std::ios::in))
{
    //this->iofs(filepath.c_str());
    //this->ifs = std::ifstream(filepath.c_str(), std::ios::in);
    this->state = FILE_NOT_OPEN;
}

NormalFile::NormalFile()
{
    this->state = FILE_NOT_OPEN;
}

NormalFile::~NormalFile()
{
    //delete StreamReader::get_instance();
    if (this->state == FILE_OPEN){
        if(this->fd.to_int() > 0){
            //std::cout << "fd:" << this->fd << std::endl;
            ::close(fd.to_int());
        }
    }
}


NormalFile* NormalFile::from_filepath(std::string const &filepath, std::ios_base::openmode option)
{
    StreamReader *ireader = StreamReader::get_instance();
    if((option & std::ios::out) ||
        ((option & std::ios::in) && (Utility::is_regular_file(filepath))))
    {
        return (new NormalFile(ireader, filepath, option));
    }
    ERROR("NormalFile::from_filepath() can't read file:" + filepath);
    throw std::invalid_argument("NormalFile::from_filepath() can't read file");
}

int NormalFile::open()
{
    if (this->state != FILE_NOT_OPEN){
        ERROR("NormalFile::open() state is  not FILE_NOT_OPEN");
        throw std::runtime_error("NormalFile::open() state is  not FILE_NOT_OPEN");
    }

    this->iofs.open(this->filepath.c_str(), this->option);
    if (this->iofs.is_open()){
        this->state = FILE_OPEN;
    }else{
        ERROR("can't open file:" + this->filepath);
        this->state = FILE_ERROR;
        throw std::runtime_error("can't open file");
    }

    return (0);
}

int NormalFile::close()
{
    if (this->state == FILE_OPEN){
        if(this->fd.to_int() > 0){
            return ::close(fd.to_int());
        }
    }
    return (-1);
}

int NormalFile::read(char **buf, size_t size)
{
    //return (0);
    FileDiscriptor tmp_fd = FileDiscriptor::from_int(0);
    return (this->ireader->read(fd, *buf, size, &(this->iofs)));
}


bool NormalFile::can_write()
{
    return (Utility::is_writable_file(this->filepath));
}

bool NormalFile::can_read()
{
    return (Utility::is_redable_file(this->filepath));
}

size_t NormalFile::size()
{
    std::cout << "normal file:" <<  std::endl;
    std::cout << "normal file:" <<  std::endl;
    std::cout << "normal file:" <<  std::endl;
    std::cout << "normal file:" <<  std::endl;
    std::cout << "normal file:" <<  std::endl;
    return (Utility::get_file_size(this->filepath));
}

bool NormalFile::is_chunk()
{
    return (false);
}


int NormalFile::remove()
{
    return (std::remove(this->filepath.c_str()));
}

std::string const &NormalFile::path()
{
    return (this->filepath);
}

/*
std::string NormalFile::to_str()
{
    return filepath;
}
*/
