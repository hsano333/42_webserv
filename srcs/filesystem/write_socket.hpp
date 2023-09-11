#ifndef WRITE_SOCKET_HPP
#define WRITE_SOCKET_HPP
#include "iwriter.hpp"
#include "file_discriptor.hpp"

class WriteSocket : public IWriter
{
    public:
        WriteSocket();
        ~WriteSocket();
        int write(FileDiscriptor fd, char const *buf, size_t size);
};

#endif
