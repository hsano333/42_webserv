#ifndef MULTI_NORMAL_FILE_HPP
#define MULTI_NORMAL_FILE_HPP

#include "file_discriptor.hpp"
#include "ireader.hpp"
#include "webserv_file.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>

class MultiNormalFile
{
    public:
        ~MultiNormalFile();
        static MultiNormalFile* from_directory_path(std::string const &directory_path, std::ios_base::openmode mode);
        int open();
        int close();
        int read(char **buf, size_t size);
        int write(char **buf, size_t size);
        int save(char *data, size_t size);
        //bool can_read();
        //bool can_write();

        FileDiscriptor const &fd();
        bool is_chunk();
        void set_chunk(bool flag);
        size_t size();
        int remove();
        std::string const &path();
    private:
        MultiNormalFile(IReader* ireader, std::string const &directory_path, std::ios_base::openmode mode);
        FileDiscriptor fd_;
        IReader *ireader;
        //IWriter *writer;
        std::string const &directory_path;
        std::ios_base::openmode option;
        FileState   state;
        std::fstream iofs;
        bool is_chunked_;
        //std::ios_base::openmode mode;

};

#endif
