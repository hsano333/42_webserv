#include "normal_writer.hpp"
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

int NormalWriter::write(FileDiscriptor fd, char const *buf, size_t size, std::fstream *ifs)
{
    (void)ifs;
    return ::write(fd.to_int(), buf, size);
}
