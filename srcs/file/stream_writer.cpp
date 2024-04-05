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

int StreamWriter::write(FileDiscriptor const &fd, char const *buf, size_t size, std::fstream *ifs)
{
    DEBUG("StreamWriter::write: size:" + Utility::to_string(size));
    (void)fd;
    (void)buf;
    size_t before = ifs->tellp();
    ifs->write(buf, size);
    if (ifs->fail() && !ifs->eof()){
        ifs->close();
        return -1;
    }
    size_t after = ifs->tellp();
    //ifs->close();
    DEBUG("StreamWriter:: written size:" + Utility::to_string(after - before));
    return (after - before);
}
