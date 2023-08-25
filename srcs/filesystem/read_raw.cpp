#include "read_raw.hpp"
#include "global.hpp"
#include <unistd.h>

ReadRaw::ReadRaw()
{
}
ReadRaw::~ReadRaw()
{
}

int ReadRaw::iread(FileDiscriptor fd, char *buf)
{
    return read(fd.to_int(), buf, MAX_READ_SIZE);
}
