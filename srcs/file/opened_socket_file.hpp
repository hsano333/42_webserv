
#ifndef OPENED_SOCKET_FILE_HPP
#define OPENED_SOCKET_FILE_HPP
#include <dirent.h>
#include <string>
#include <vector>
#include "file.hpp"
#include "ireader.hpp"
#include "iwriter.hpp"
#include "status_code.hpp"
#include "buffer_controller.hpp"

class OpenedSocketFile : public File
{
    public:
        OpenedSocketFile();
        ~OpenedSocketFile();
        static OpenedSocketFile* from_fd(FileDiscriptor fd, IWriter* iwriter, IReader* ireader);
        static OpenedSocketFile* from_fd(IReader* ireader, FileDiscriptor fd);
        static OpenedSocketFile* from_fd(IWriter* iwriter, FileDiscriptor fd);
        int open();
        int close();
        int read(char **buf, size_t size);
        int write(char **buf, size_t size);
        int save(char *data, size_t size);
        bool can_read();
        size_t size();
        bool is_chunk();
        int remove();
        std::string const &path();
    private:
        FileState   state;
        std::string text;
        FileDiscriptor fd;
        IReader *reader;
        IWriter *writer;
        BufferController buffer;
};

#endif
