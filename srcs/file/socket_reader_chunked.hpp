#ifndef SOCKET_READER_CHUNKED_HPP
#define SOCKET_READER_CHUNKED_HPP
#include "ireader.hpp"

class SocketReaderChunked : public IReader
{
    public:
        static SocketReaderChunked *get_instance();
        ~SocketReaderChunked();
        int read(FileDiscriptor const &fd, char *buf, size_t size, std::fstream *ifs);
    private:
        static SocketReaderChunked *singleton;
        SocketReaderChunked();
        size_t chunked_size;

};

#endif
