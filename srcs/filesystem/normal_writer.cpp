#include "normal_writer.hpp"
#include <sys/types.h>
#include <sys/socket.h>

NormalWriter::NormalWriter()
{
    ;
}
NormalWriter::~NormalWriter()
{
}

int NormalWriter::write(FileDiscriptor fd, char const *buf, size_t size)
{
    return ::write(fd.to_int(), buf, size);
}
