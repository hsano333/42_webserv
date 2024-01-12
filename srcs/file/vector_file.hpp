
#ifndef VECTOR_FILE_HPP
#define VECTOR_FILE_HPP
#include <dirent.h>
#include <vector>
#include <string>
#include "file.hpp"
#include "ireader.hpp"
#include "status_code.hpp"

class VectorFile : public File
{
    public:
        VectorFile();
        ~VectorFile();
        static VectorFile* from_buf_size(size_t buf_size);
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
        VectorFile(size_t max_buf_size);
        FileState   state;
        std::string path_;
        std::vector<char> buf;
        size_t max_buf_size;
        std::vector<char> tmp_buf;
        //char buf_c[MAX_BUF];
};

#endif
