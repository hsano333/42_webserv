
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

/*
PipeFile* PipeFile::from_fd(IReader* reader, FileDiscriptor fd)
{
    DEBUG("PipeFile::from_fd reader()");
    PipeFile *file = new PipeFile();
    file->fd = fd;
    file->reader = reader;
    file->state = FILE_NOT_OPEN;
    return (file);
}

PipeFile* PipeFile::from_fd(IWriter* iwriter, FileDiscriptor fd)
{
    DEBUG("PipeFile::from_fd writer()");
    PipeFile *file = new PipeFile();
    file->fd = fd;
    file->writer = iwriter;
    file->state = FILE_NOT_OPEN;
    return (file);
}

int PipeFile::open()
{
    DEBUG("PipeFile::open()");
    this->state = FILE_OPEN;
    return 0;
}
*/



int PipeFile::read(char **buf, size_t max_size)
{
    if (!(this->state == FILE_OPEN || this->state == FILE_READING)){
        return (0);
    }
    //return (this->reader->read(this->fd, *buf, max_size, NULL));
    int tmp =  (this->reader->read(this->fd, *buf, max_size, NULL));
    DEBUG("PipeFile::read() result=" + Utility::to_string(tmp));
    if(tmp <= 0){
        this->state = FILE_CLOSE;
        this->close();
        return (0);
    }
    this->state = FILE_READING;
    DEBUG("PipeFile::read() read size=" + Utility::to_string(tmp));
    return (tmp);
}

int PipeFile::write(char **buf, size_t size)
{
    DEBUG("PipeFile::write() size=" + Utility::to_string(size));
    if (!(this->state == FILE_OPEN || this->state == FILE_WRITING)){
        return (0);
    }
    /*
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
    this->state = FILE_WRITING;
    return (result);
    //DEBUG("write rval=" + Utility::to_string(rval));
    //return (rval);
}

/*
int PipeFile::open()
{
    DEBUG("PipeFile::open()");
    this->state = FILE_OPEN;
    return 0;
}

*/
int PipeFile::close()
{
    DEBUG("PipeFile::close(");
    this->state = FILE_CLOSE;
    if(this->state != FILE_CLOSE){
        this->fd.close();
    }
    return (0);
}

/*
int PipeFile::save(char *buf, size_t size)
{
    return (this->buffer.save(buf, size));
}

int PipeFile::save(char **buf, size_t size)
{
    (void)size;
    if (this->state != FILE_OPEN){
        return (0);
    }
    return this->reader->read(this->fd, *buf, size, NULL);
}
*/



/*
int PipeFile::close()
{
    DEBUG("PipeFile::close(");
    this->state = FILE_CLOSE;
    return (0);
}

bool PipeFile::can_read()
{
    return (true);
}
*/

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

/*
bool PipeFile::is_chunk()
{
    return (false);
}
*/

