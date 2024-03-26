#include "multi_normal_file.hpp"
#include "global.hpp"
#include "webserv_file.hpp"
#include "stream_reader.hpp"


MultiNormalFile::MultiNormalFile(std::string const &directory_path, std::string const &boundary, FileDiscriptor const &fd) :
    fd_(fd),
    directory_path_(directory_path),
    boundary_(boundary),
    file(NULL)
    //option(option)
{
    this->state_ = FILE_NOT_OPEN;
}

MultiNormalFile* MultiNormalFile::from_directory_path(std::string const &directory_path,std::string const &boundary, FileDiscriptor const &fd)
{
    return (new MultiNormalFile(directory_path, boundary, fd));
}


std::string const &MultiNormalFile::content_type_boundary()
{
    return (this->boundary_);
}

std::string const &MultiNormalFile::directory_path()
{
    return (this->directory_path_);
}


/*
MultiNormalFile* MultiNormalFile::from_directory_path(std::string const &directory_path, std::ios_base::openmode mode)
{
    StreamReader *ireader = StreamReader::get_instance();
    if((mode & std::ios::in) == std::ios::in){
        MYINFO("MultiNormalFile::from_filepath() Reader");
        return (new MultiNormalFile(ireader, directory_path, mode));
    }
    ERROR("MultiNormalFile::from_filepath() can't use as write file");
    throw std::invalid_argument("MultiNormalFile::from_filepath() can't use as write file");
}
*/

FileState const &MultiNormalFile::state()
{
    return (this->state_);
}

int MultiNormalFile::open()
{
    DEBUG("MultiNormalFile::open()");
    return (true);
    /*
    if (this->state != FILE_NOT_OPEN){
        WARNING("MultiNormalFile::open() state is  not FILE_NOT_OPEN");
    }

    this->iofs.open(this->path().c_str(), this->option);
    if (this->iofs.is_open()){
        this->state = FILE_OPEN;
        return (0);
    }else{
        ERROR("can't open file:" + this->path());
        this->state = FILE_ERROR;
        throw std::runtime_error("can't open file");
    }
    */
    return (-1);
}

int MultiNormalFile::close()
{
    //DEBUG("MultiNormalFile::close() fd:" + Utility::to_string(fd.to_int()));
    if (this->state_ != FILE_NOT_OPEN){
        if(this->fd().to_int() > 0){
            return ::close(this->fd().to_int());
        }
    }
    return (-1);
}

int MultiNormalFile::read(char **buf, size_t size)
{
    return (this->ireader->read(this->fd(), *buf, size, &(this->iofs)));
}

int MultiNormalFile::write(char **buf, size_t size)
{
    if(file){
        return (file->write(buf, size));
    }
    return (-1);
}

std::string const &MultiNormalFile::path()
{
    return (this->directory_path_);
}

FileDiscriptor const &MultiNormalFile::fd()
{
    return (this->fd_);
}

bool MultiNormalFile::is_chunk()
{
    return (this->is_chunked_);
}

void MultiNormalFile::set_chunk(bool flag)
{
    this->is_chunked_ = flag;
}
