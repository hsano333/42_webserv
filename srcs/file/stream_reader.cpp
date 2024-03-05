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

int StreamReader::read(FileDiscriptor const &fd, char *buf, size_t size, std::fstream *ifs)
{
    DEBUG("StreamReader::read: size:" + Utility::to_string(size));
    (void)fd;
    ifs->read(buf, size);
    if (ifs->fail() && !ifs->eof()){
        ifs->close();
        return (-1);
    }
    return (ifs->gcount());
}
