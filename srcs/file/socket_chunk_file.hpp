
#ifndef SOCKET_CHUNK_FILE_HPP
#define SOCKET_CHUNK_FILE_HPP
#include <dirent.h>
#include <string>
#include <vector>
#include "webserv_file.hpp"
#include "ireader.hpp"
#include "iwriter.hpp"
#include "status_code.hpp"
#include "buffer_controller.hpp"
#include "webserv_file.hpp"

class SocketChunkFile
{
    public:
        ~SocketChunkFile();
        static SocketChunkFile* from_file(FileDiscriptor const &fd, WebservFile *file);
        //static SocketChunkFile* from_fd(IReader* ireader, FileDiscriptor fd);
        //static SocketChunkFile* from_fd(IWriter* iwriter, FileDiscriptor fd);
        //int open();
        //int close();
        int read(char **buf, size_t size);
        int write(char **buf, size_t size);
        //int save(char *data, size_t size);
        bool can_read();
        //bool is_chunk();
        FileState   state;

        size_t chunked_size();
        void set_chunked_size(size_t size);
        bool is_chunked();
        void set_is_chunked(bool chunked);
        std::vector<char> &buf();
        size_t            buf_size();
        size_t            buf_p();
        //void              clear_buf_size();
        void              set_buf(char *data, size_t size);
        //void              append_buf(char *data, size_t size);

    private:
        SocketChunkFile(FileDiscriptor const &fd, WebservFile *file);
        FileDiscriptor const &fd;
        IReader *reader;
        IWriter *writer;
        BufferController buffer;
        size_t chunked_size_;
        bool   is_chunked_;
        WebservFile *file_;
        std::vector<char> buf_;
        //size_t buf_size_;
        size_t buf_p_;
        //std::vector<char> buf;
        //size_t            chunked_size;
        //std::vector<char> tmp_buf;
};

#endif
