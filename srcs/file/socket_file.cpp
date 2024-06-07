
#include "socket_file.hpp"
#include "global.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;

SocketFile::SocketFile(FileDiscriptor const &fd, WebservFile *file) :
                            state(FILE_NOT_OPEN),
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

SocketFile::~SocketFile()
{
    DEBUG("SocketFile Destructor");
}

SocketFile* SocketFile::from_fd(FileDiscriptor const &fd, IWriter* writer, IReader* reader)
{
    DEBUG("SocketFile::from_fd fd:" + fd.to_string());
    SocketFile *file = new SocketFile(fd, NULL);
    file->reader = reader;
    file->writer = writer;
    file->state = FILE_NOT_OPEN;
    return (file);
}

SocketFile* SocketFile::from_file(FileDiscriptor const &fd, WebservFile *buf_file,  IWriter* writer, IReader* reader)
{
    DEBUG("SocketFile::from_fd fd:" + fd.to_string());
    SocketFile *file = new SocketFile(fd, buf_file);
    file->reader = reader;
    file->writer = writer;
    file->state = FILE_NOT_OPEN;
    return (file);
}

int SocketFile::read(char **buf, size_t max_size)
{
    int tmp =  (this->reader->read(this->fd, *buf, max_size, NULL));
    return (tmp);
}

int SocketFile::write(char **buf, size_t size)
{
    int result = (this->writer->write(this->fd, *buf, size, NULL));
    if(result > 0){
        total_write_size += result;
    }
    return (result);
}

int SocketFile::close()
{
    DEBUG("SocketFile::close(");
    this->state = FILE_CLOSE;
    this->fd.close();
    return (0);
}

BufferController const &SocketFile::get_buffer()
{
    DEBUG("SocketFile::get_buffer");
    return (this->buffer);
}

bool SocketFile::can_read()
{
    return (true);
}

size_t SocketFile::chunked_size()
{
    return (this->chunked_size_);
}

void SocketFile::set_chunked_size(size_t size)
{
    this->chunked_size_ = size;
}

bool SocketFile::is_chunk()
{
    return (this->is_chunked_);
}

void SocketFile::set_is_chunk(bool flag)
{
    this->is_chunked_ = flag;
}

size_t SocketFile::size()
{
    DEBUG("SocketFile::size()");
    return (this->total_write_size);
}

WebservFile *SocketFile::file()
{
    return (this->file_);
}

void SocketFile::clear_file()
{
    this->file_ = NULL;
}


