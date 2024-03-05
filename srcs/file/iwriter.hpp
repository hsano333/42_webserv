#ifndef IWRITER_HPP
#define IWRITER_HPP
#include "file_discriptor.hpp"
#include "iwriter.hpp"

class IWriter
{
    public:
        IWriter(){};
        virtual ~IWriter(){};
        virtual int write(FileDiscriptor const &fd, char const *buf, size_t size, std::fstream *ofs) = 0;
};

#endif
