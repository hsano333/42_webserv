#ifndef SOCKET_READER_HPP
#define SOCKET_READER_HPP
#include "ireader.hpp"

class SocketReader : public IReader
{
    public:
        static SocketReader *get_instance();
        ~SocketReader();
        int read(FileDiscriptor const &fd, char *buf, size_t size, std::fstream *ifs);
    private:
        static SocketReader *singleton;
        SocketReader();

};

#endif
