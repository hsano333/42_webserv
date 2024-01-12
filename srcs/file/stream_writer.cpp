#include "stream_writer.hpp"
#include "global.hpp"
#include <unistd.h>


StreamWriter::StreamWriter()
{
}

StreamWriter::~StreamWriter()
{
    //delete (singleton);
}

StreamWriter* StreamWriter::singleton = NULL;
StreamWriter *StreamWriter::get_instance()
{
    if (StreamWriter::singleton == NULL){
        singleton = new StreamWriter();
    }
    return (singleton);
}

int StreamWriter::write(FileDiscriptor fd, char const *buf, size_t size, std::fstream *ifs)
{
    DEBUG("StreamWriter::write: size:" + Utility::to_string(size));
    DEBUG("StreamWriter::write: ifs:" + Utility::to_string(ifs));
    (void)fd;
    (void)buf;
    std::string tmp = "abcdefg";
    size_t before = ifs->tellp();
    ifs->write(buf, size);
    if (ifs->fail() && !ifs->eof()){
        ifs->close();
        return -1;
    }
    DEBUG("StreamWriter::write:before:" + Utility::to_string(before));
    size_t after = ifs->tellp();
    DEBUG("StreamWriter::write:after:" + Utility::to_string(after));
    return (after - before);
}
