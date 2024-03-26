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
        static MultiNormalFile* from_directory_path(std::string const &directory_path, std::string const &boundary, FileDiscriptor const &fd);
        //static MultiNormalFile* from_fd(std::string const &directory_path, FileDiscriptor const &fd);
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
        FileState   const &state();
        std::string const &content_type_boundary();
        std::string const &directory_path();
        void set_content_type_boundary(std::string &boundary);
    private:
        //MultiNormalFile(IReader* ireader, std::string const &directory_path, std::ios_base::openmode mode);
        MultiNormalFile(std::string const &directory_path,std::string const &boundary, FileDiscriptor const &fd);
        FileDiscriptor const &fd_;
        IReader *ireader;
        //IWriter *writer;
        std::string const &directory_path_;
        std::string const &boundary_;
        //std::ios_base::openmode option;
        FileState   state_;
        std::fstream iofs;
        bool is_chunked_;
        WebservFile *file;
        //std::ios_base::openmode mode;

};

#endif
