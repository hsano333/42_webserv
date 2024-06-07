#include "file_manager.hpp"
#include "global.hpp"
#include "log.hpp"

FileManager::FileManager()
{
    ;
}

FileManager::~FileManager()
{
    ;
}

void FileManager::insert(FileDiscriptor const &fd, WebservFile *file)
{
    DEBUG("FileManager::push_back() fd=" + fd.to_string());
    DEBUG("FileManager::push_back() file=" + Utility::to_string(file));

    if(file == NULL){
        DEBUG("FileManager::push_back() file is NULL");
        return ;
    }

    std::map<FileDiscriptor, std::set<WebservFile*> >::iterator ite = this->file_list.begin();
    std::map<FileDiscriptor, std::set<WebservFile*> >::iterator end = this->file_list.end();

    bool exist_flag = false;
    while(ite != end)
    {
        if(ite->first == fd){
            ite->second.insert(file);
            exist_flag = true;
            break;
        }
        ite++;
    }
    if(!exist_flag){
        std::set<WebservFile *> file_vector;
        this->file_list.insert(std::make_pair(fd, file_vector));
        this->file_list[fd].insert(file);
    }

}


void FileManager::erase(FileDiscriptor const &fd)
{
    DEBUG("FileManager::erase fd=" + fd.to_string());
    std::map<FileDiscriptor, std::set<WebservFile*> >::iterator tmp_ite = this->file_list.find(fd);
    if(tmp_ite == this->file_list.end()){
        return ;
    }
    std::set<WebservFile*> tmp_vector = tmp_ite->second;

    std::set<WebservFile*>::iterator ite = tmp_vector.begin();
    std::set<WebservFile*>::iterator end = tmp_vector.end();

    std::map<void*, int> file_counts;

    while(ite != end)
    {
        WebservFile *file = *ite;
        void *address = file->file_address();
        DEBUG("address info :" + Utility::to_string(address));
        std::map<void *, int>::iterator cnt_ite = file_counts.find(address);
        if(cnt_ite == file_counts.end()){
            file_counts.insert(std::make_pair(address, 1));
        }else{
            cnt_ite->second++;
        }
        ite++;
    }

    ite = tmp_vector.begin();
    end = tmp_vector.end();

    int cnt = 0;
    while(ite != end)
    {

        cnt++;
        WebservFile *file = *ite;
        if(*ite == NULL){
            ite++;
            continue;
        }

        void *address = file->file_address();
        int file_cnt = file_counts[address];

        if(file_cnt == 1){
            DEBUG("delete file:" + Utility::to_string(address));
            file->close();
            file->delete_file();
        }else{
            file_counts[address]--;
            DEBUG("already delete file:" + Utility::to_string(address));
        }

        delete file;
        ite++;
    }

    this->file_list.erase(fd);
}
