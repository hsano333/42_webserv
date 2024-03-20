#include "multi_normal_file.hpp"
#include "global.hpp"
#include "webserv_file.hpp"
#include "stream_reader.hpp"


MultiNormalFile::MultiNormalFile(IReader* ireader, std::string const &directory_path, std::ios_base::openmode option) :
    ireader(ireader),
    directory_path(directory_path),
    option(option)
{
    this->state = FILE_NOT_OPEN;
}


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

int MultiNormalFile::open()
{
    DEBUG("MultiNormalFile::open()");
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
    return (-1);
}

int MultiNormalFile::close()
{
    //DEBUG("MultiNormalFile::close() fd:" + Utility::to_string(fd.to_int()));
    if (this->state != FILE_NOT_OPEN){
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
    (void)buf;
    (void)size;
    ERROR("can't use write in MultiNormalFile");
    throw std::invalid_argument("can't use write in MultiNormalFile");
    //return (this->iwriter->write(this->fd, *buf, size, &(this->iofs)));
    return 0;
}

std::string const &MultiNormalFile::path()
{
    return (this->directory_path);

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
