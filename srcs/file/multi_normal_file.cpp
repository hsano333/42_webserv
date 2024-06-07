#include "multi_normal_file.hpp"
#include "global.hpp"
#include "webserv_file.hpp"
#include "stream_reader.hpp"


MultiNormalFile::MultiNormalFile(std::string const &directory_path, std::string const &boundary, FileDiscriptor const &fd) :
    fd_(fd),
    directory_path_(directory_path),
    boundary_(boundary),
    is_chunked_(false),
    file(NULL),
    completed_(false),
    register_file_flag_(false),
    uploaded_number_of_files_(0)
{
    this->state_ = FILE_NOT_OPEN;
}

MultiNormalFile::~MultiNormalFile()
{
    ;
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

FileState const &MultiNormalFile::state()
{
    return (this->state_);
}

int MultiNormalFile::open()
{
    DEBUG("MultiNormalFile::open()");
    this->state_ = FILE_OPEN;
    return (true);
}

int MultiNormalFile::close()
{
    DEBUG("MultiNormalFile::close() ");
    if (this->file){
        DEBUG("MultiNormalFile::close():close ");
        this->file->close();
    }
    return (-1);
}

int MultiNormalFile::read(char **buf, size_t size)
{
    return (this->ireader->read(this->fd(), *buf, size, &(this->iofs)));
}

int MultiNormalFile::write(char **buf, size_t size)
{
    DEBUG("MultiNormalFile::write size:" + Utility::to_string(size));
    if(file){
        return (file->write(buf, size));
    }
    return (-1);
}

std::string const &MultiNormalFile::path()
{
    if(this->file){
        this->file->path();
    }
    return (this->directory_path_);
}

FileDiscriptor const &MultiNormalFile::fd()
{
    return (this->fd_);
}

bool MultiNormalFile::is_chunk()
{
    DEBUG("MultiNormalFile::is_chunk():" + Utility::to_string(this->is_chunked_));
    return (this->is_chunked_);
}

void MultiNormalFile::set_chunk(bool flag)
{
    this->is_chunked_ = flag;
}


void MultiNormalFile::register_file(WebservFile *file, int code)
{
    if(this->uploaded_files_.find(file) == this->uploaded_files_.end()){
        this->uploaded_files_.insert(std::make_pair(file, StatusCode::from_int(code)));
    }else{
        this->uploaded_files_[file] = StatusCode::from_int(code);
    }
}

bool MultiNormalFile::can_write_file()
{
    DEBUG("MultiNormalFile::can_write_file");
    if(this->file == NULL){
        return (false);
    }
    StatusCode &status = this->uploaded_files_[this->file];
    if(status.to_int() == 200){
        return (true);
    }
    return (false);
}

void MultiNormalFile::set_file(WebservFile *file)
{
    this->file = file;
}

bool MultiNormalFile::exist_file()
{
    if(this->file){
        return (true);
    }
    return (false);
}


void MultiNormalFile::clear_buf()
{
    this->buf.clear();
}

void MultiNormalFile::set_buf(const char *buf, size_t size)
{
    if(size > MAX_BUF){
        ERROR("memory size is exceed");
        throw std::runtime_error("memory size is exceed");
    }

    this->buf.resize(size);
    for(size_t i=0;i<size;i++){
        this->buf[i] = buf[i];
    }
}

void MultiNormalFile::add_buf(const char *buf, size_t size)
{
    size_t cur_size = this->buf.size();
    if(cur_size+size > MAX_BUF){
        ERROR("memory size is exceed");
        throw std::runtime_error("memory size is exceed");
    }

    this->buf.resize(cur_size+size);
    for(size_t i=0;i<size;i++){
        this->buf[i+cur_size] = buf[i];
    }
}

void MultiNormalFile::set_completed(bool flag)
{
    DEBUG("MultiNormalFile::set_completed:" + Utility::to_string(flag));
    this->completed_ = flag;
    this->uploaded_number_of_files_ = this->uploaded_files_.size();
}

bool MultiNormalFile::completed()
{
    DEBUG("MultiNormalFile::completed():" + Utility::to_string(this->completed_));
    return (this->completed_);
}

std::map<WebservFile *, StatusCode> &MultiNormalFile::uploaded_files(){
    return (this->uploaded_files_);

}

bool MultiNormalFile::register_file_flag(){
    return (this->register_file_flag_);
}

void MultiNormalFile::set_register_file_flag(bool flag){
    this->register_file_flag_ = flag;
}

size_t MultiNormalFile::uploaded_number_of_files(){
    return (this->uploaded_number_of_files_);
}

void MultiNormalFile::set_uploaded_number_of_files(size_t size){
    this->uploaded_number_of_files_ = size;
}

