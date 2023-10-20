#ifndef IREADER_HPP
#define IREADER_HPP
#include "file_discriptor.hpp"

class IReader
{
    public:
        IReader(){};
        virtual ~IReader(){};
        virtual int read(FileDiscriptor fd, char *buf, size_t size, std::ifstream *ifs) = 0;
};

#endif
