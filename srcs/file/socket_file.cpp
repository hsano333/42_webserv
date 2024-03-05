
#include "socket_file.hpp"
#include "global.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;

SocketFile::SocketFile(FileDiscriptor const &fd) :
                            fd(fd),
                            reader(NULL),
                            writer(NULL)
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
    SocketFile *file = new SocketFile(fd);
    //file->fd = fd;
    file->reader = reader;
    file->writer = writer;
    file->state = FILE_NOT_OPEN;
    DEBUG("SocketFile: file->fd :" + file->fd.to_string());
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
*/

int SocketFile::open()
{
    DEBUG("SocketFile::open()");
    this->state = FILE_OPEN;
    return 0;
}

int SocketFile::read(char **buf, size_t max_size)
{
    DEBUG("SocketFile::read() size=" + Utility::to_string(max_size));
    /*
    size_t size = this->buffer.retrieve(buf, max_size);
    if(size > 0){
        return (size);
    }
    */

    if (this->state != FILE_OPEN){
        return (0);
    }

    this->state = FILE_COMPLETED_READ;
    return this->reader->read(this->fd, *buf, max_size, NULL);
}

int SocketFile::write(char **buf, size_t size)
{
    DEBUG("SocketFile::write() size=" + Utility::to_string(size));
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

    int rval = this->writer->write(this->fd, *buf, size, NULL);

    DEBUG("write rval=" + Utility::to_string(rval));
    return (rval);
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



int SocketFile::close()
{
    DEBUG("SocketFile::close(");
    this->state = FILE_CLOSE;
    return (0);
}

bool SocketFile::can_read()
{
    return (true);
}

size_t SocketFile::size()
{
    return (this->text.size());
}

bool SocketFile::is_chunk()
{
    return (false);
}


int SocketFile::remove()
{
    // error file is not exist
    return (-1);
}

std::string const &SocketFile::path()
{
    return (this->text);
}
