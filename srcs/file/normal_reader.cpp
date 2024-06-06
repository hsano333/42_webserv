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
    //int rval = ::read(fd.to_int(), buf, size);
    //
    
    if(!ifs || ifs->fail()){
        return -1;
    }
    ifs->read(buf, size);
    if(ifs->fail()){
        return -1;
    }
    DEBUG("NormalReader::read size:" + Utility::to_string(ifs->gcount()));
    return (ifs->gcount());
}
