#include "read_raw.hpp"
#include "global.hpp"
#include <unistd.h>

ReadRaw::ReadRaw()
{
}
ReadRaw::~ReadRaw()
{
}

int ReadRaw::read(FileDiscriptor fd, char *buf, size_t size)
{
    return ::read(fd.to_int(), buf, size);
}
