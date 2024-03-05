#include "normal_reader.hpp"
#include "global.hpp"
#include <unistd.h>


NormalReader::NormalReader()
{
}

NormalReader::~NormalReader()
{
    //delete (singleton);
}

NormalReader *NormalReader::singleton = NULL;
NormalReader *NormalReader::get_instance()
{
    if (NormalReader::singleton == NULL){
        singleton = new NormalReader();
    }
    return (singleton);
}

int NormalReader::read(FileDiscriptor const &fd, char *buf, size_t size, std::fstream *ifs)
{
    DEBUG("NormalReader::read fd:" + fd.to_string());
    (void)ifs;
    return ::read(fd.to_int(), buf, size);
}
