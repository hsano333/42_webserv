#ifndef WRITE_RAW_HPP
#define WRITE_RAW_HPP
#include "iwriter.hpp"
#include "file_discriptor.hpp"
#include <unistd.h>

class WriteRaw : public IWriter
{
    public:
        WriteRaw();
        ~WriteRaw();
        int write(FileDiscriptor fd, char const *buf, size_t size);
};

#endif
