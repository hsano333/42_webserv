
#ifndef READ_SOCKET_HPP
#define READ_SOCKET_HPP
#include "ireader.hpp"

class ReadSocket : IReader
{
    public:
        ReadSocket();
        ~ReadSocket();
        int iread(FileDiscriptor fd, char *buf);

};

#endif
