#ifndef IWRITER_HPP
#define IWRITER_HPP
#include "file_discriptor.hpp"

class IWriter
{
    public:
        IWriter(){};
        virtual ~IWriter(){};
        virtual int write(FileDiscriptor fd, char const *buf, size_t size, std::fstream *ifs) = 0;
};

#endif
