#include "raw_file_reader.hpp"
#include "file_discriptor.hpp"

void RawFileReader::read_file(Filepath filepath, char *data, size_t size)
{
    return read();
}

void RawFileReader::read_file(FileDiscriptor const &fd, char *data, size_t size)
{
    return read(fd, data, size);
}
