
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
        int read(char **buf, size_t size);
        bool can_read();
        size_t size();
        bool is_chunk();
        int remove();
        FileState   state;

    private:
        std::string text;
        std::vector<char> tmp_buf;

};

#endif
