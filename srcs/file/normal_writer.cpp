#include "normal_writer.hpp"
#include "global.hpp"
#include <sys/types.h>
#include <sys/socket.h>


NormalWriter::NormalWriter()
{
    ;
}
NormalWriter::~NormalWriter()
{
    //delete (singleton);
}

NormalWriter* NormalWriter::singleton = NULL;
NormalWriter *NormalWriter::get_instance()
{
    if (NormalWriter::singleton == NULL){
        singleton = new NormalWriter();
    }
    return (singleton);
}

int NormalWriter::write(FileDiscriptor const &fd, char const *buf, size_t size, std::fstream *ifs)
{
    DEBUG("NormalWriter::write fd=" + fd.to_string());
    if(buf == NULL){
        DEBUG("NormalWriter::write buf is NULL");
    }

    if(!ifs){
        int val = ::write(fd.to_int(), buf, size);
        return val;
    }

    if(ifs->fail()){
        return -1;
    }

    ifs->write(buf, size);
    if(ifs->fail()){
        return -1;
    }
    return (ifs->gcount());
    (void)ifs;
}
