#ifndef FILE_LOADER_HPP
#define FILE_LOADER_HPP
#include "file_discriptor.hpp"

class IReader
{
    public:
        IReader(){};
        virtual ~IReader(){};
        virtual int iread(FileDiscriptor fd, char *buf) = 0;
};

#endif
