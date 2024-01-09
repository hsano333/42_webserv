
#ifndef OBJECT_FILE_HPP
#define OBJECT_FILE_HPP
#include <dirent.h>
#include <vector>
#include <string>
#include "file.hpp"
#include "ireader.hpp"
#include "status_code.hpp"

class ObjectFile : public File
{
    public:
        ObjectFile();
        ~ObjectFile();
        static ObjectFile* from_buf_size(size_t buf_size);
        int open();
        int close();
        int read(char **buf, size_t size);
        int write(char **buf, size_t size);
        bool can_read();
        size_t size();
        bool is_chunk();
        int remove();
        std::string const &path();

    private:
        ObjectFile(size_t max_buf_size);
        FileState   state;
        std::string text;
        std::vector<char> buf;
        size_t max_buf_size;
        //char buf_c[MAX_BUF];
};

#endif
