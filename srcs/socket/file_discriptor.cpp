#include "file_discriptor.hpp"
#include "global.hpp"
#include "utility.hpp"
#include <unistd.h>
#include <string>


FileDiscriptor::~FileDiscriptor()
{
    ;
}

FileDiscriptor::FileDiscriptor() : fd(-1)
{
    ;
}

FileDiscriptor::FileDiscriptor(int fd) : fd(fd)
{
    ;
}

FileDiscriptor::FileDiscriptor(FileDiscriptor const &fd)
{
    DEBUG("FileDiscriptor::FileDiscriptor");
    this->fd = fd.fd;
}

FileDiscriptor& FileDiscriptor::operator=(FileDiscriptor const &fd)
{
    DEBUG("FileDiscriptor& FileDiscriptor::operator=");
    this->fd = fd.fd;
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
    DEBUG("FileDiscriptor::to_int() const");
    return (this->fd);
}

int FileDiscriptor::to_int()
{
    DEBUG("FileDiscriptor::to_int()");
    return (this->fd);
}

std::string const FileDiscriptor::to_string() const
{
    return (Utility::to_string(fd));
}


void FileDiscriptor::close() const
{
    if(this->fd >= 0){
        ::close(this->fd);
    }
}

bool FileDiscriptor::operator==(const FileDiscriptor &fd) const
{
    DEBUG("FileDiscriptor::operator==");
    DEBUG("FileDiscriptor:: this->fd =" + Utility::to_string(this->fd));
    DEBUG("FileDiscriptor:: fd =" + fd.to_string());
    DEBUG("FileDiscriptor:: test fd =" + Utility::to_string(1));
    DEBUG("FileDiscriptor:: test No.2 fd =" + Utility::to_string(1730577014));
    DEBUG("FileDiscriptor:: fd =" + Utility::to_string(fd.to_int()));
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


