
#include "opened_socket_file.hpp"
#include "global.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;

OpenedSocketFile::OpenedSocketFile() :
                            reader(NULL),
                            writer(NULL)
{
    ;
}

OpenedSocketFile::~OpenedSocketFile()
{
}


OpenedSocketFile* OpenedSocketFile::from_fd(FileDiscriptor fd, IWriter* writer, IReader* reader)
{
    DEBUG("OpenedSocketFile::from_fd");
    OpenedSocketFile *file = new OpenedSocketFile();
    file->fd = fd;
    file->reader = reader;
    file->writer = writer;
    file->state = FILE_NOT_OPEN;
    return (file);
}

OpenedSocketFile* OpenedSocketFile::from_fd(IReader* reader, FileDiscriptor fd)
{
    DEBUG("OpenedSocketFile::from_fd reader()");
    OpenedSocketFile *file = new OpenedSocketFile();
    file->fd = fd;
    file->reader = reader;
    file->state = FILE_NOT_OPEN;
    return (file);
}

OpenedSocketFile* OpenedSocketFile::from_fd(IWriter* iwriter, FileDiscriptor fd)
{
    DEBUG("OpenedSocketFile::from_fd writer()");
    OpenedSocketFile *file = new OpenedSocketFile();
    file->fd = fd;
    file->writer = iwriter;
    file->state = FILE_NOT_OPEN;
    return (file);
}

int OpenedSocketFile::open()
{
    DEBUG("OpenedSocketFile::open()");
    this->state = FILE_OPEN;
    return 0;
}

int OpenedSocketFile::read(char **buf, size_t max_size)
{
    DEBUG("OpenedSocketFile::read() size=" + Utility::to_string(max_size));
    size_t size = this->buffer.retrieve(buf, max_size);
    if(size > 0){
        return (size);
    }

    if (this->state != FILE_OPEN){
        return (0);
    }

    this->state = FILE_COMPLETED_READ;
    return this->reader->read(this->fd, *buf, max_size, NULL);
}

int OpenedSocketFile::write(char **buf, size_t size)
{
    DEBUG("OpenedSocketFile::write() size=" + Utility::to_string(size));
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

int OpenedSocketFile::save(char *buf, size_t size)
{
    return (this->buffer.save(buf, size));
}

/*
int OpenedSocketFile::save(char **buf, size_t size)
{
    (void)size;
    if (this->state != FILE_OPEN){
        return (0);
    }
    return this->reader->read(this->fd, *buf, size, NULL);
}
*/



int OpenedSocketFile::close()
{
    this->state = FILE_CLOSE;
    return (0);
}

bool OpenedSocketFile::can_read()
{
    return (true);
}

size_t OpenedSocketFile::size()
{
    return (this->text.size());
}

bool OpenedSocketFile::is_chunk()
{
    return (false);
}


int OpenedSocketFile::remove()
{
    // error file is not exist
    return (-1);
}

std::string const &OpenedSocketFile::path()
{
    return (this->text);
}
