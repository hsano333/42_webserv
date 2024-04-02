
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
#include "global.hpp"

class SocketFile
{
    public:
        SocketFile(FileDiscriptor const &fd, WebservFile *file);
        ~SocketFile();
        static SocketFile* from_fd(FileDiscriptor const &fd, IWriter* iwriter, IReader* ireader);
        static SocketFile* from_file(FileDiscriptor const &fd, WebservFile *file, IWriter* iwriter, IReader* ireader);
        int read(char **buf, size_t size);
        int write(char **buf, size_t size);
        WebservFile *file();
        bool can_read();
        FileState   state;

        size_t chunked_size();
        void set_chunked_size(size_t size);
        bool is_chunked();
        void set_is_chunked(bool chunked);
        size_t size();
        void clear_file();
    private:
        FileDiscriptor const &fd;
        WebservFile *file_;
        IReader *reader;
        IWriter *writer;
        BufferController buffer;
        size_t chunked_size_;
        bool   is_chunked_;
        size_t total_write_size;
        //std::vector<char> tmp_buf;
};

namespace BufferFunc{
    template <class FileT>
    int write(FileT *file, char **data, size_t size){
        DEBUG("BufferFunc::write()");
        if(file->file()){
            int result = (file->file()->write(data, size));
            file->clear_file();
            return (result);
        }
        return (file->write(data, size));
    }
    template <class FileT>
    int read(FileT *file, char **data, size_t size){
        DEBUG("BufferFunc::read()");
        if(file->file()){
            int result = (file->file()->read(data, size));
            file->clear_file();
            return (result);
        }
        return (file->read(data, size));
    }
}

#endif
