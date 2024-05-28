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
    DEBUG("FileManager::push_back() file=" + Utility::to_string(file));

    if(file == NULL){
        DEBUG("FileManager::push_back() file is NULL");
        return ;
    }
    printf("file manager file=%p\n", file);
    DEBUG("filemanager:: file=" + Utility::to_string(file));

    std::map<FileDiscriptor, std::set<WebservFile*> >::iterator ite = this->file_list.begin();
    std::map<FileDiscriptor, std::set<WebservFile*> >::iterator end = this->file_list.end();


    //DEBUG("No.1 this->file_list size=" + Utility::to_string(this->file_list.size()));

    bool exist_flag = false;
    while(ite != end)
    {
        cout << "fd:" << ite->first.to_string() << endl;
        //DEBUG("ite->second size=" + Utility::to_string(ite->second.size()));

        if(ite->first == fd){
        //DEBUG("ite->second No.1 size=" + Utility::to_string(ite->second.size()));
            ite->second.insert(file);
        //DEBUG("ite->second No.2 size=" + Utility::to_string(ite->second.size()));
            exist_flag = true;
            break;
        }
        ite++;
    }
    //DEBUG("ite->second No.3 size=" + Utility::to_string(ite->second.size()));
    if(!exist_flag){
        std::set<WebservFile *> file_vector;
        this->file_list.insert(std::make_pair(fd, file_vector));
        this->file_list[fd].insert(file);
    }
    //DEBUG("No.2 this->file_list size=" + Utility::to_string(this->file_list.size()));

    std::map<FileDiscriptor, std::set<WebservFile*> >::iterator ite2 = this->file_list.begin();
    std::map<FileDiscriptor, std::set<WebservFile*> >::iterator end2 = this->file_list.end();

    while(ite2 != end2)
    {
        //DEBUG("No.2 ite->second size=" + Utility::to_string(ite2->second.size()));
        ite2++;
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

    std::vector<void *> deleted_address_vector;
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
        DEBUG("child file address:" + Utility::to_string(address));

        std::vector<void *>::iterator address_ite = deleted_address_vector.begin();
        std::vector<void *>::iterator address_end = deleted_address_vector.end();
        while(address_ite != address_end){
            if(*address_ite == address){
                break;
            }
            address_ite++;
        }

        file->close();
        if(address_ite == deleted_address_vector.end()){
            DEBUG("delete file:" + Utility::to_string(address));
            file->delete_file();
            DEBUG("end  deleting file:" + Utility::to_string(address));
            deleted_address_vector.push_back(address);
        }else{
            DEBUG("already delete file:" + Utility::to_string(address));
        }



        DEBUG("FileManager::erase file=" + Utility::to_string(file));
        delete file;
        ite++;

        /*
        cnt++;
        //File *tmp_file = *ite;
        if(*ite == NULL){
            continue;
        }
        (*ite)->close();
        DEBUG("FileManager::erase file=" + Utility::to_string(*ite));
        delete *ite;
        ite++;
        */

            /*
        cnt++;
        //File *tmp_file = *ite;
        WebservFile *file = *ite;
        if(file == NULL){
            ite++;
            continue;
        }
        void *address = file->file_address();
        DEBUG("child file address:" + Utility::to_string(address));

        std::vector<void *>::iterator address_ite = deleted_address_vector.begin();
        std::vector<void *>::iterator address_end = deleted_address_vector.end();
        while(address_ite != address_end){
            if(*address_ite == address){
                break;
            }
            address_ite++;
        }
        //std::vector<void *>::iterator deleted_address = std::find(deleted_address_vector.begin(), deleted_address_vector.end(), address);

        if(address_ite == deleted_address_vector.end()){
            file->delete_file();
            deleted_address_vector.push_back(address);
        }else{
            DEBUG("already delete file:" + Utility::to_string(address));

        }
        file->close();
        DEBUG("delete WebservFile");
        delete file;


        DEBUG("FileManager::erase file=" + Utility::to_string(*ite));
        ite++;
        */
    }
    this->file_list.erase(fd);
}
