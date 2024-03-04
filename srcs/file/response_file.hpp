
#ifndef RESPONSE_FILE_HPP
#define RESPONSE_FILE_HPP
#include <dirent.h>
#include <string>
#include <vector>
#include "webserv_file.hpp"
#include "ireader.hpp"
#include "status_code.hpp"

class ResponseFile
{
    public:
        ResponseFile();
        ~ResponseFile();
        static ResponseFile* from_status_code(StatusCode const &status_code);
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
