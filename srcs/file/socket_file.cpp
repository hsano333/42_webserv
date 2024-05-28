
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

/*
SocketFile* SocketFile::from_fd(IReader* reader, FileDiscriptor fd)
{
    DEBUG("SocketFile::from_fd reader()");
    SocketFile *file = new SocketFile();
    file->fd = fd;
    file->reader = reader;
    file->state = FILE_NOT_OPEN;
    return (file);
}

SocketFile* SocketFile::from_fd(IWriter* iwriter, FileDiscriptor fd)
{
    DEBUG("SocketFile::from_fd writer()");
    SocketFile *file = new SocketFile();
    file->fd = fd;
    file->writer = iwriter;
    file->state = FILE_NOT_OPEN;
    return (file);
}

int SocketFile::open()
{
    DEBUG("SocketFile::open()");
    this->state = FILE_OPEN;
    return 0;
}
*/



int SocketFile::read(char **buf, size_t max_size)
{
    DEBUG("SocketFile::read() max_read_size=" + Utility::to_string(max_size));
    //return (this->reader->read(this->fd, *buf, max_size, NULL));
    int tmp =  (this->reader->read(this->fd, *buf, max_size, NULL));
    DEBUG("SocketFile::read() read size=" + Utility::to_string(tmp));
    return (tmp);
}

int SocketFile::write(char **buf, size_t size)
{
    DEBUG("SocketFile::write() size=" + Utility::to_string(size));
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
    int result = (this->writer->write(this->fd, *buf, size, NULL));
    if(result > 0){
        total_write_size += result;
    }
    return (result);
    //DEBUG("write rval=" + Utility::to_string(rval));
    //return (rval);
}

/*
int SocketFile::open()
{
    DEBUG("SocketFile::open()");
    this->state = FILE_OPEN;
    return 0;
}

*/
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


/*
int SocketFile::save(char *buf, size_t size)
{
    return (this->buffer.save(buf, size));
}

int SocketFile::save(char **buf, size_t size)
{
    (void)size;
    if (this->state != FILE_OPEN){
        return (0);
    }
    return this->reader->read(this->fd, *buf, size, NULL);
}
*/



/*
int SocketFile::close()
{
    DEBUG("SocketFile::close(");
    this->state = FILE_CLOSE;
    return (0);
}

*/
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

/*
bool SocketFile::is_chunk()
{
    return (false);
}
*/

