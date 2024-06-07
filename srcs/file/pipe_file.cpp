
#include "pipe_file.hpp"
#include "global.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;

PipeFile::PipeFile(FileDiscriptor const &fd, WebservFile *file) :
                            fd(fd),
                            file_(file),
                            reader(NULL),
                            writer(NULL),
                            chunked_size_(0),
                            is_chunked_(false),
                            total_write_size(0)
{
    ;
}

PipeFile::~PipeFile()
{
    this->close();
    DEBUG("PipeFile Destructor");
}

PipeFile* PipeFile::from_fd(FileDiscriptor const &fd, IWriter* writer, IReader* reader)
{
    DEBUG("PipeFile::from_fd fd:" + fd.to_string());
    PipeFile *file = new PipeFile(fd, NULL);
    file->reader = reader;
    file->writer = writer;
    file->state = FILE_OPEN;
    return (file);
}

PipeFile* PipeFile::from_file(FileDiscriptor const &fd, WebservFile *buf_file,  IWriter* writer, IReader* reader)
{
    DEBUG("PipeFile::from_fd fd:" + fd.to_string());
    PipeFile *file = new PipeFile(fd, buf_file);
    file->reader = reader;
    file->writer = writer;
    file->state = FILE_OPEN;
    return (file);
}

int PipeFile::read(char **buf, size_t max_size)
{
    if (!(this->state == FILE_OPEN || this->state == FILE_READING)){
        return (0);
    }
    int tmp =  (this->reader->read(this->fd, *buf, max_size, NULL));
    if(tmp <= 0){
        this->state = FILE_CLOSE;
        this->close();
        return (0);
    }
    this->state = FILE_READING;
    return (tmp);
}

int PipeFile::write(char **buf, size_t size)
{
    if (!(this->state == FILE_OPEN || this->state == FILE_WRITING)){
        return (0);
    }
    int result = (this->writer->write(this->fd, *buf, size, NULL));
    if(result > 0){
        total_write_size += result;
    }
    this->state = FILE_WRITING;
    return (result);
}

int PipeFile::close()
{
    DEBUG("PipeFile::close(");
    this->state = FILE_CLOSE;
    if(this->state != FILE_CLOSE){
        this->fd.close();
    }
    return (0);
}


size_t PipeFile::chunked_size()
{
    return (this->chunked_size_);
}

void PipeFile::set_chunked_size(size_t size)
{
    this->chunked_size_ = size;
}

bool PipeFile::is_chunk()
{
    return (this->is_chunked_);
}

void PipeFile::set_is_chunk(bool flag)
{
    this->is_chunked_ = flag;
}

size_t PipeFile::size()
{
    return (this->total_write_size);
}

WebservFile *PipeFile::file()
{
    return (this->file_);
}

void PipeFile::clear_file()
{
    this->file_ = NULL;
}


