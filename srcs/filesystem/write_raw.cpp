#include "write_raw.hpp"
#include <sys/types.h>
#include <sys/socket.h>

WriteRaw::WriteRaw()
{
    ;
}
WriteRaw::~WriteRaw()
{
}

int WriteRaw::write(FileDiscriptor fd, char const *buf, size_t size)
{
    return ::write(fd.to_int(), buf, size);
}
