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
    if(buf == NULL){
        DEBUG("NormalWriter::write buf is NULL");
        return -1;
    }

    if(!ifs){
        if(fd.to_int() > 0){
            int val = ::write(fd.to_int(), buf, size);
            return val;
        }
        return -1;
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
