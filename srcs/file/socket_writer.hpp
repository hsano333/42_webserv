#ifndef SOCKET_WRITER_HPP
#define SOCKET_WRITER_HPP
#include "iwriter.hpp"
#include "file_discriptor.hpp"

class SocketWriter : public IWriter
{
    public:
        static SocketWriter *get_instance();
        ~SocketWriter();
        int write(FileDiscriptor const &fd, char const *buf, size_t size, std::fstream *ifs);
    private:
        static SocketWriter *singleton;
        SocketWriter();
};

#endif
