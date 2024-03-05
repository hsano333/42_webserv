
#ifndef OPENED_SOCKET_FILE_HPP
#define OPENED_SOCKET_FILE_HPP
#include <dirent.h>
#include <string>
#include <vector>
#include "webserv_file.hpp"
#include "ireader.hpp"
#include "iwriter.hpp"
#include "status_code.hpp"
#include "buffer_controller.hpp"

class SocketFile
{
    public:
        SocketFile(FileDiscriptor const &fd);
        ~SocketFile();
        static SocketFile* from_fd(FileDiscriptor const &fd, IWriter* iwriter, IReader* ireader);
        //static SocketFile* from_fd(IReader* ireader, FileDiscriptor fd);
        //static SocketFile* from_fd(IWriter* iwriter, FileDiscriptor fd);
        int open();
        int close();
        int read(char **buf, size_t size);
        int write(char **buf, size_t size);
        //int save(char *data, size_t size);
        bool can_read();
        size_t size();
        bool is_chunk();
        int remove();
        std::string const &path();
        FileState   state;
    private:
        std::string text;
        FileDiscriptor const &fd;
        IReader *reader;
        IWriter *writer;
        BufferController buffer;
};

#endif
