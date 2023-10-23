#include "directory_file.hpp"
#include "global.hpp"
#include <sstream>
#include <iomanip>

DirectoryFile::DirectoryFile()
{
    ;
}

DirectoryFile::~DirectoryFile()
{
    if (this->state == FILE_OPEN){
        ::closedir(dir);
    }
}

DirectoryFile* DirectoryFile::from_path(std::string const &path, std::string const &relative_path, std::string const &domain)
{
    if(Utility::is_directory(path) == false){
        ERROR("DirectoryFile::from_path() not directory:" + path);
        throw std::invalid_argument("DirectoryFile::from_path() not directory");
    }
    DirectoryFile *dir = new DirectoryFile();
    dir->path = path;
    dir->state = FILE_NOT_OPEN;

    //size_t pos = path.find(root);
    //pos += root.size();

    dir->relative_path = relative_path;
    dir->domain = domain;
    return (dir);
}

int DirectoryFile::open()
{
    if (this->state != FILE_NOT_OPEN){
        ERROR("DirectoryFile::open() state is  not FILE_NOT_OPEN");
        throw std::runtime_error("DirectoryFile::open() state is  not FILE_NOT_OPEN");
    }
    DIR *tmp_dir = opendir(this->path.c_str());
    if (tmp_dir == NULL){
        ERROR("DirectoryFile::open() error");
        throw std::runtime_error("DirectoryFile::open() error");
    }
    this->state = FILE_OPEN;
    this->dir = tmp_dir;
    return 0;
}


int DirectoryFile::read(char **buf, size_t size)
{
    (void)size;
    if (this->state == FILE_OPEN){

        size_t pos = this->path.find(this->relative_path);
        //size_t pos = this->dir.rfind("/");
        //std::string up_dir = this->dir.rfind("/");
        std::string relative = this->path.substr(pos);
        std::string up_dir = "http://" + this->domain + "/";
        pos = relative.rfind("/");
        if(pos != std::string::npos){
            up_dir += relative.substr(0, pos);
        }
        

        //std::string up_dir = relative.
        std::string directory_path = WRITE_OPEN1 + relative + WRITE_OPEN2 + up_dir + WRITE_OPEN3 + "<br>";
        Utility::memcpy(*buf, &(directory_path[0]), directory_path.size());
        this->state = FILE_READING;
        return directory_path.size();
    }

    if (this->state != FILE_READING){
        ERROR("DirectoryFile::read() state is not READING");
        throw std::runtime_error("DirectoryFile::read() state is not READING");
    }
    struct dirent *dirr;
    //dirr = readdir(this->dir);
    while(1){
        dirr = readdir(this->dir);
        if (!dirr){
            return (0);
        }
        std::string tmp = dirr->d_name;
        if (!(tmp == "." || tmp == "..")){
            break;
        }
    }

    std::string uri = "http://" + this->domain + "/" + relative_path + "/" + dirr->d_name;
    std::string filepath = this->path + "/" +  dirr->d_name;
    size_t filesize = Utility::get_file_size(filepath);
    std::string filesize_str = Utility::adjust_filesize(filesize);
    if (Utility::is_directory(filepath)){
        filesize_str = "-";
    }
    std::string date = Utility::get_file_updated_date(this->path);
    std::string tmp = WRITE_READING1 + uri + WRITE_READING2 + dirr->d_name + WRITE_READING3;

    stringstream ss;
    ss  << std::setfill(' ') << std::left << std::setw(128) << tmp << date << " " << std::setfill(' ') << std::right << std::setw(8) << filesize_str << "<br>";
    //ss  << std::setfill('\r') << std::left << std::setw(64) << tmp << "<br>";
    std::string returned_str = ss.str();

    Utility::memcpy(*buf, &(returned_str[0]), returned_str.size());
    return (returned_str.size());
}

int DirectoryFile::close()
{
    if (this->state != FILE_OPEN){
        ERROR("DirectoryFile::close() state is  not FILE_OPEN");
        throw std::runtime_error("DirectoryFile::close() state is  not FILE_OPEN");
    }
    int tmp = closedir(this->dir);
    if (tmp == 0){
        this->state = FILE_CLOSE;
    }
    return (tmp);
}

bool DirectoryFile::can_read()
{
    return (Utility::is_redable_directory(this->path));
}

size_t DirectoryFile::size()
{
    std::cout << "directory file????:"  << std::endl;
    std::cout << "directory file????:"  << std::endl;
    std::cout << "directory file????:"  << std::endl;
    std::cout << "directory file????:"  << std::endl;
    return (0);
}

bool DirectoryFile::is_chunk()
{
    return (true);
}
