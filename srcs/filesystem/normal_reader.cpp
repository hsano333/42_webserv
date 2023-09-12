#include "normal_reader.hpp"
#include "global.hpp"
#include <unistd.h>

NormalReader::NormalReader()
{
}
NormalReader::~NormalReader()
{
}

int NormalReader::read(FileDiscriptor fd, char *buf, size_t size)
{
    return ::read(fd.to_int(), buf, size);
}
