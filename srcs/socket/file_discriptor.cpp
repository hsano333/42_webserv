#include "file_discriptor.hpp"
#include "global.hpp"
#include "utility.hpp"
#include <unistd.h>
#include <string>


FileDiscriptor::~FileDiscriptor()
{
    ;
}

FileDiscriptor::FileDiscriptor() : fd(-1), is_close_(false)
{
    ;
}

FileDiscriptor::FileDiscriptor(int fd) : fd(fd), is_close_(false)
{
    ;
}

FileDiscriptor::FileDiscriptor(FileDiscriptor const &fd)
{
    //DEBUG("FileDiscriptor::FileDiscriptor");
    this->fd = fd.fd;
    this->is_close_ = fd.is_close_;
}

FileDiscriptor& FileDiscriptor::operator=(FileDiscriptor const &fd)
{
    //DEBUG("FileDiscriptor& FileDiscriptor::operator=");
    this->fd = fd.fd;
    this->is_close_ = fd.is_close_;
    return (*this);
}

FileDiscriptor FileDiscriptor::from_int(int fd)
{
    if(fd < 0){
        ERROR("fd is must > 0, but fd:" + Utility::to_string(fd));
        throw std::invalid_argument("fd is must > 0");
    }
    return FileDiscriptor(fd);
}

int FileDiscriptor::to_int() const
{
    //DEBUG("FileDiscriptor::to_int() const");
    return (this->fd);
}

int FileDiscriptor::to_int()
{
    //DEBUG("FileDiscriptor::to_int()");
    return (this->fd);
}

std::string const FileDiscriptor::to_string() const
{
    return (Utility::to_string(fd));
}

bool FileDiscriptor::is_close() const
{
    return (this->is_close_);
}

void FileDiscriptor::close() const
{
    if(this->fd >= 0 && this->is_close_ == false){
        ::close(this->fd);
        //this->is_close_ = true;
    }
}

bool FileDiscriptor::operator==(const FileDiscriptor &fd) const
{
    return (this->fd == fd.to_int());
}


bool FileDiscriptor::operator>=(FileDiscriptor const &fd) const
{
    return (this->fd >= fd.fd);
}

bool FileDiscriptor::operator>(FileDiscriptor const &fd) const
{
    return (this->fd > fd.fd);
}

bool FileDiscriptor::operator<(FileDiscriptor const &fd) const
{
    return (this->fd < fd.fd);
}

bool FileDiscriptor::operator<=(FileDiscriptor const &fd) const
{
    return (this->fd <= fd.fd);
}

bool FileDiscriptor::operator==(int fd) const
{
    return (this->fd == fd);
}

bool FileDiscriptor::operator!=(int fd) const
{
    return (this->fd != fd);
}

bool FileDiscriptor::operator>=(int fd) const
{
    return (this->fd >= fd);
}

bool FileDiscriptor::operator>(int fd) const
{
    return (this->fd > fd);
}

bool FileDiscriptor::operator<(int fd) const
{
    return (this->fd < fd);
}

bool FileDiscriptor::operator<=(int fd) const
{
    return (this->fd <= fd);
}

//friend std::ostream& operator<<(std::ostream& stream, const FileDiscriptor &fd);

std::ostream& operator<<(std::ostream& os, const FileDiscriptor &fd)
{
    std::string out = Utility::to_string(fd.to_int());
    os << out;
    return (os);
}



