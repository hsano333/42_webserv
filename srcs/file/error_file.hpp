
#ifndef ERROR_FILE_HPP
#define ERROR_FILE_HPP
#include <dirent.h>
#include <vector>
#include <string>
#include "webserv_file.hpp"
#include "ireader.hpp"
#include "status_code.hpp"

class ErrorFile
{
    public:
        ErrorFile();
        ~ErrorFile();
        static ErrorFile* from_status_code(StatusCode const &status_code);
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
        std::vector<char> tmp_buf;

};

#endif
