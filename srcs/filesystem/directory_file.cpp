#include "directory_file.hpp"
#include "global.hpp"

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

DirectoryFile* DirectoryFile::from_path(std::string const &path)
{
    if(Utility::is_directory(path) == false){
        ERROR("DirectoryFile::from_path() not directory:" + path);
    }
    DirectoryFile *dir = new DirectoryFile();
    dir->path = path;
    dir->state = FILE_NOT_OPEN;
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

int DirectoryFile::read(char *buf, size_t size)
{
    (void)size;
    if (this->state != FILE_OPEN){
        ERROR("DirectoryFile::read() state is  not FILE_OPEN");
        throw std::runtime_error("DirectoryFile::read() state is  not FILE_OPEN");
    }
    struct dirent *dirr = readdir(this->dir);
    if (dirr == NULL){
        return (-1);
    }
    Utility::memcpy(buf, dirr->d_name, Utility::strlen(dirr->d_name));
    return 0;
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
    return (0);
}
