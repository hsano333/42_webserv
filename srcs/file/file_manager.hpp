#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <map>
#include <set>
#include "webserv_file.hpp"
#include "file_discriptor.hpp"


typedef WebservFile*(* FP_FUNC)();

class FileManager
{
    public:
        FileManager();
        ~FileManager();
        //WebservFile *make( FP_FUNC func);
        void insert(FileDiscriptor const &fd, WebservFile *event);
        void erase(FileDiscriptor const &fd);

    private:
        std::map<FileDiscriptor, std::set<WebservFile*> > file_list;
};

#endif
