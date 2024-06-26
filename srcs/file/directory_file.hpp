#ifndef DIRECTORY_FILE_HPP
#define DIRECTORY_FILE_HPP
#include <dirent.h>
#include <string>
#include <vector>
#include "webserv_file.hpp"
#include "ireader.hpp"

#define WRITE_OPEN1 "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1252\"><title>Index of Directory</title></head><body><h1>Index of "
#define WRITE_OPEN2 "</h1><hr><pre><a href=\""
#define WRITE_OPEN3 "\">../</a>"

#define WRITE_READING1 "<div style=\" display:flex;justify-content: space-between; padding:0px 30px; max-width:1000px \"><a href=\"" 
#define WRITE_READING2 "\">"
#define WRITE_READING3 "</a>"


class DirectoryFile
{
    public:
        DirectoryFile();
        ~DirectoryFile();
        static DirectoryFile* from_path(std::string const &path, std::string const &relative_path, std::string const &domain);
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
        DIR *dir;
        std::string path_;
        std::string root;
        std::string relative_path;
        std::string domain;
        FileState   state;
        std::vector<char> tmp_buf;
};

#endif
