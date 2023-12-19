
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

OpenedSocketFile* OpenedSocketFile::from_fd(IReader* reader, FileDiscriptor fd)
{
    OpenedSocketFile *file = new OpenedSocketFile();
    file->fd = fd;
    file->reader = reader;
    file->state = FILE_NOT_OPEN;
    return (file);
}

OpenedSocketFile* OpenedSocketFile::from_fd(IWriter* iwriter, FileDiscriptor fd)
{
    OpenedSocketFile *file = new OpenedSocketFile();
    file->fd = fd;
    file->writer = iwriter;
    file->state = FILE_NOT_OPEN;
    return (file);
}

int OpenedSocketFile::open()
{
    this->state = FILE_OPEN;
    return 0;
}

int OpenedSocketFile::read(char **buf, size_t size)
{
    (void)size;
    if (this->state != FILE_OPEN){
        return (0);
    }
    return this->reader->read(this->fd, *buf, size, NULL);
}

int OpenedSocketFile::write(char **buf, size_t size)
{
    (void)size;
    if (this->state != FILE_OPEN){
        return (0);
    }
    return this->writer->write(this->fd, *buf, size, NULL);
}


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
