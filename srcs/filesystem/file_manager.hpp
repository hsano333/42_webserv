#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <map>
#include <set>
#include "file.hpp"
#include "file_discriptor.hpp"

class FileManager
{
    public:
        FileManager();
        ~FileManager();
        void insert(FileDiscriptor fd, File *event);
        void erase(FileDiscriptor fd);

    private:
        std::map<FileDiscriptor, std::set<File*> > file_list;
};

#endif
