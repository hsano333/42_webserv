
#ifndef READ_SOCKET_HPP
#define READ_SOCKET_HPP
#include "ireader.hpp"

class ReadSocket : public IReader
{
    public:
        ReadSocket();
        ~ReadSocket();
        int read(FileDiscriptor fd, char *buf, size_t size);

};

#endif
