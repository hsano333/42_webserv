
#ifndef READ_SOCKET_HPP
#define READ_SOCKET_HPP
#include "ireader.hpp"

class ReadSocket : IReader
{
    public:
        ReadSocket();
        ~ReadSocket();
        int iread(int fd, char *buf);

};

#endif
