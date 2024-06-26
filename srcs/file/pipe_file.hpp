
#ifndef PIPE_FILE_HPP
#define PIPE_FILE_HPP
#include <dirent.h>
#include <string>
#include <vector>
#include "webserv_file.hpp"
#include "ireader.hpp"
#include "iwriter.hpp"
#include "status_code.hpp"
#include "buffer_controller.hpp"
#include "global.hpp"

class PipeFile
{
    public:
        PipeFile(FileDiscriptor const &fd, WebservFile *file);
        ~PipeFile();
        static PipeFile* from_fd(FileDiscriptor const &fd, IWriter* iwriter, IReader* ireader);
        static PipeFile* from_file(FileDiscriptor const &fd, WebservFile *file, IWriter* iwriter, IReader* ireader);
        int read(char **buf, size_t size);
        int write(char **buf, size_t size);
        int open();
        int close();
        WebservFile *file();
        FileState   state;

        size_t chunked_size();
        void set_chunked_size(size_t size);
        bool is_chunk();
        void set_is_chunk(bool chunked);
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
};

#endif
