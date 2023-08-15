#include "read_raw.hpp"
#include "global.hpp"
#include <unistd.h>

ReadRaw::ReadRaw()
{
}
ReadRaw::~ReadRaw()
{
}

int ReadRaw::iread(int fd, char *buf)
{
    return read(fd, buf, MAX_READ_SIZE);
}
