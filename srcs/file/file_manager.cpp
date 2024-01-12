#include "file_manager.hpp"

FileManager::FileManager()
{
    ;
}

FileManager::~FileManager()
{
    ;
}

void FileManager::insert(FileDiscriptor fd, File *event)
{
    if(event == NULL){
        return ;
    }

    std::map<FileDiscriptor, std::set<File*> >::iterator ite = this->file_list.begin();
    std::map<FileDiscriptor, std::set<File*> >::iterator end = this->file_list.end();
    while(ite != end)
    {
        if(ite->first == fd){
            ite->second.insert(event);
        }
        ite++;
    }
}


void FileManager::erase(FileDiscriptor fd)
{
    std::map<FileDiscriptor, std::set<File*> >::iterator tmp_ite = this->file_list.find(fd);
    if(tmp_ite == this->file_list.end()){
        return ;
    }
    std::set<File*> tmp_set = tmp_ite->second;

    std::set<File*>::iterator ite = tmp_set.begin();
    std::set<File*>::iterator end = tmp_set.end();

    while(ite != end)
    {
        File *tmp_file = *ite;
        tmp_file->close();
        delete tmp_file;
    }

    this->file_list.erase(fd);
}
