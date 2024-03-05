#ifndef IREADER_HPP
#define IREADER_HPP
#include "file_discriptor.hpp"
#include "ireader.hpp"

class IReader
{
    public:
        IReader(){};
        virtual ~IReader(){};
        virtual int read(FileDiscriptor const &fd, char *buf, size_t size, std::fstream *ifs) = 0;
};

#endif
