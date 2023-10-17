#ifndef SOCKET_READER_HPP
#define SOCKET_READER_HPP
#include "ireader.hpp"

class SocketReader : public IReader
{
    public:
        static SocketReader *get_instance();
        ~SocketReader();
        int read(FileDiscriptor fd, char *buf, size_t size);
    private:
        static SocketReader *singleton;
        SocketReader();

};

#endif
