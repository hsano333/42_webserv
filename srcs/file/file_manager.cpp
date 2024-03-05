#include "file_manager.hpp"
#include "global.hpp"

FileManager::FileManager()
{
    ;
}

FileManager::~FileManager()
{
    ;
}

/*
WebservFile *FileManager::make(FP_FUNC func)
{

    func();
    WebservFile *file = NULL;
    return (file);
}
*/

void FileManager::insert(FileDiscriptor const &fd, WebservFile *file)
{
    DEBUG("FileManager::push_back() fd=" + fd.to_string());

    if(file == NULL){
        DEBUG("FileManager::push_back() file is NULL");
        return ;
    }
    printf("file manager file=%p\n", file);
    DEBUG("filemanager:: file=" + Utility::to_string(file));


    std::map<FileDiscriptor, std::vector<WebservFile*> >::iterator ite = this->file_list.begin();
    std::map<FileDiscriptor, std::vector<WebservFile*> >::iterator end = this->file_list.end();


    DEBUG("No.1 this->file_list size=" + Utility::to_string(this->file_list.size()));

    bool exist_flag = false;
    while(ite != end)
    {
        cout << "fd:" << ite->first.to_string() << endl;
        DEBUG("ite->second size=" + Utility::to_string(ite->second.size()));

        if(ite->first == fd){
        DEBUG("ite->second No.1 size=" + Utility::to_string(ite->second.size()));
            ite->second.push_back(file);
        DEBUG("ite->second No.2 size=" + Utility::to_string(ite->second.size()));
            exist_flag = true;
            break;
        }
        ite++;
    }
        DEBUG("ite->second No.3 size=" + Utility::to_string(ite->second.size()));
    if(!exist_flag){
        std::vector<WebservFile *> file_vector;
        this->file_list.insert(std::make_pair(fd, file_vector));
        this->file_list[fd].push_back(file);
    }
    DEBUG("No.2 this->file_list size=" + Utility::to_string(this->file_list.size()));

    std::map<FileDiscriptor, std::vector<WebservFile*> >::iterator ite2 = this->file_list.begin();
    std::map<FileDiscriptor, std::vector<WebservFile*> >::iterator end2 = this->file_list.end();

    while(ite2 != end2)
    {
        DEBUG("No.2 ite->second size=" + Utility::to_string(ite2->second.size()));
        ite2++;
    }

}


void FileManager::erase(FileDiscriptor const &fd)
{
    DEBUG("FileManager::erase fd=" + fd.to_string());
    std::map<FileDiscriptor, std::vector<WebservFile*> >::iterator tmp_ite = this->file_list.find(fd);
    if(tmp_ite == this->file_list.end()){
        return ;
    }
    std::vector<WebservFile*> tmp_vector = tmp_ite->second;

    std::vector<WebservFile*>::iterator ite = tmp_vector.begin();
    std::vector<WebservFile*>::iterator end = tmp_vector.end();

    int cnt = 0;
    while(ite != end)
    {
        cnt++;
        //File *tmp_file = *ite;
        if(*ite == NULL){
            continue;
        }
        (*ite)->close();
        delete *ite;
        ite++;
    }
    this->file_list.erase(fd);
}
