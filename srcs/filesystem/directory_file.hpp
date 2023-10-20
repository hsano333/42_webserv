#ifndef DIRECTORY_FILE_HPP
#define DIRECTORY_FILE_HPP
#include <dirent.h>
#include <string>
#include "file.hpp"
#include "ireader.hpp"

class DirectoryFile : public File
{
    public:
        DirectoryFile();
        ~DirectoryFile();
        static DirectoryFile* from_path(std::string const &path);
        int open();
        int close();
        int read(char *buf, size_t size);
        bool can_read();
        size_t size();
    private:
        DIR *dir;
        std::string path;
        FileState   state;

};

#endif
