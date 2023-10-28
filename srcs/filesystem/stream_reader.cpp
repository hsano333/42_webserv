#include "stream_reader.hpp"
#include "global.hpp"
#include <unistd.h>


StreamReader::StreamReader()
{
}

StreamReader::~StreamReader()
{
    //delete (singleton);
}

StreamReader* StreamReader::singleton = NULL;
StreamReader *StreamReader::get_instance()
{
    if (StreamReader::singleton == NULL){
        singleton = new StreamReader();
    }
    return (singleton);
}

int StreamReader::read(FileDiscriptor fd, char *buf, size_t size, std::fstream *ifs)
{
    (void)fd;
    ifs->read(buf, size);
    if(*ifs){
        return (size);
    }
    return (ifs->gcount());
}
