#ifndef DIRECTORY_FILE_HPP
#define DIRECTORY_FILE_HPP
#include <dirent.h>
#include <string>
#include "file.hpp"
#include "ireader.hpp"

#define WRITE_OPEN1 "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1252\"><title>Index of Directory</title></head><body><h1>Index of "
#define WRITE_OPEN2 "</h1><hr><pre><a href=\""
#define WRITE_OPEN3 "\">../</a>"

#define WRITE_READING1 "<a href=\"" 
#define WRITE_READING2 "\">"
#define WRITE_READING3 "</a>"


class DirectoryFile : public File
{
    public:
        DirectoryFile();
        ~DirectoryFile();
        static DirectoryFile* from_path(std::string const &path, std::string const &relative_path, std::string const &domain);
        int open();
        int close();
        int read(char **buf, size_t size);
        bool can_read();
        size_t size();
        bool is_chunk();
    private:
        DIR *dir;
        std::string path;
        std::string root;
        std::string relative_path;
        std::string domain;
        FileState   state;
};

#endif
