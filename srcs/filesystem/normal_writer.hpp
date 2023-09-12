#ifndef NORMAL_WRITER_HPP
#define NORMAL_WRITER_HPP
#include "iwriter.hpp"
#include "file_discriptor.hpp"
#include <unistd.h>

class  NormalWriter: public IWriter
{
    public:
        NormalWriter();
        ~NormalWriter();
        int write(FileDiscriptor fd, char const *buf, size_t size);
};

#endif
