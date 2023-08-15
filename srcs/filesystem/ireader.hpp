#ifndef FILE_LOADER_HPP
#define FILE_LOADER_HPP

class IReader
{
    public:
        IReader(){};
        virtual ~IReader(){};
        virtual int iread(int fd, char *buf) = 0;
};

#endif
