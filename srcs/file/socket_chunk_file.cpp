
#include "socket_chunk_file.hpp"
#include "global.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;

SocketChunkFile::SocketChunkFile(FileDiscriptor const &fd, WebservFile *file) :
                            fd(fd),
                            reader(NULL),
                            writer(NULL),
                            chunked_size_(0),
                            is_chunked_(false),
                            file_(file)
{
    ;
}

SocketChunkFile::~SocketChunkFile()
{
    DEBUG("SocketChunkFile Destructor");
}

SocketChunkFile* SocketChunkFile::from_file(FileDiscriptor const &fd, WebservFile *file)
{
    DEBUG("SocketChunkFile::from_fd fd:" + fd.to_string());
    SocketChunkFile *new_file = new SocketChunkFile(fd, file);
    //file->reader = reader;
    //file->writer = writer;
    new_file->state = FILE_NOT_OPEN;
    return (new_file);
}

/*
SocketChunkFile* SocketChunkFile::from_fd(IReader* reader, FileDiscriptor fd)
{
    DEBUG("SocketChunkFile::from_fd reader()");
    SocketChunkFile *file = new SocketChunkFile();
    file->fd = fd;
    file->reader = reader;
    file->state = FILE_NOT_OPEN;
    return (file);
}

SocketChunkFile* SocketChunkFile::from_fd(IWriter* iwriter, FileDiscriptor fd)
{
    DEBUG("SocketChunkFile::from_fd writer()");
    SocketChunkFile *file = new SocketChunkFile();
    file->fd = fd;
    file->writer = iwriter;
    file->state = FILE_NOT_OPEN;
    return (file);
}

int SocketChunkFile::open()
{
    DEBUG("SocketChunkFile::open()");
    this->state = FILE_OPEN;
    return 0;
}

void SocketChunkFile::clear_buf_size()
{
    this->buf_size_ = 0;
}
*/

void SocketChunkFile::set_buf(char *data, size_t size)
{
    this->buf_.resize(size);
    for(size_t i=0;i<size;i++){
        this->buf_[i] = data[i];
    }
}

/*
void append_buf(char *data, size_t size){
    size_t buf_size = this->buf_.size();
    this->buf_.resize(buf_size + size);
    for(size_t i=0;i<size;i++){
        this->buf_[buf_size + i] = data[i];
    }
}
*/

size_t SocketChunkFile::buf_p()
{
    return (this->buf_p_);
}

size_t SocketChunkFile::buf_size()
{
    return (this->buf_.size());
}

std::vector<char> &SocketChunkFile::buf()
{
    return (this->buf_);
}

int SocketChunkFile::read(char **buf, size_t max_size)
{
    DEBUG("SocketChunkFile::read() size=" + Utility::to_string(max_size));
    return (this->file_->read(buf, max_size));
}

int SocketChunkFile::write(char **buf, size_t size)
{
    DEBUG("SocketChunkFile::write() size=" + Utility::to_string(size));
    /*
    if (!(this->state == FILE_OPEN || this->state == FILE_WRITING)){
        return (0);
    }
    DEBUG("write cgi_in=" + Utility::to_string(this->fd));
    cout << "write buf=" ;
    for(size_t i=0;i<size;i++){
        cout << (*buf)[i];
    }
    cout << endl;
    cout << "this->fd=" << this->fd << endl;
    (void)size;
    this->state = FILE_WRITING;
    //return this->writer->write(this->fd, *buf, size, NULL);

    */
    //int rval = this->writer->write(this->fd, *buf, size, NULL);
    return (this->writer->write(this->fd, *buf, size, NULL));

    //DEBUG("write rval=" + Utility::to_string(rval));
    //return (rval);
}

/*
int SocketChunkFile::save(char *buf, size_t size)
{
    return (this->buffer.save(buf, size));
}

int SocketChunkFile::save(char **buf, size_t size)
{
    (void)size;
    if (this->state != FILE_OPEN){
        return (0);
    }
    return this->reader->read(this->fd, *buf, size, NULL);
}
*/



/*
int SocketChunkFile::close()
{
    DEBUG("SocketChunkFile::close(");
    this->state = FILE_CLOSE;
    return (0);
}
*/

bool SocketChunkFile::can_read()
{
    return (true);
}

size_t SocketChunkFile::chunked_size()
{
    return (this->chunked_size_);
}

void SocketChunkFile::set_chunked_size(size_t size)
{
    this->chunked_size_ = size;
}

bool SocketChunkFile::is_chunked()
{
    return (this->is_chunked_);
}

void SocketChunkFile::set_is_chunked(bool flag)
{
    this->is_chunked_ = flag;
}

/*
bool SocketChunkFile::is_chunk()
{
    return (false);
}
*/

