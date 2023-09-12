#ifndef SOCKET_WRITER_HPP
#define SOCKET_WRITER_HPP
#include "iwriter.hpp"
#include "file_discriptor.hpp"

class SocketWriter : public IWriter
{
    public:
        SocketWriter();
        ~SocketWriter();
        int write(FileDiscriptor fd, char const *buf, size_t size);
};

#endif
