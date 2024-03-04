
#include "directory_reader.hpp"
#include "global.hpp"
#include <unistd.h>


DirectoryReader::DirectoryReader()
{
}

DirectoryReader::~DirectoryReader()
{
    delete (singleton);
}

DirectoryReader* DirectoryReader::singleton = NULL;
DirectoryReader *DirectoryReader::get_instance()
{
    if (DirectoryReader::singleton == NULL){
        singleton = new DirectoryReader();
    }
    return (singleton);
}

int DirectoryReader::read(FileDiscriptor fd, char *buf, size_t size)
{
    return ::readdir(fd.to_int(), buf, size);
}
