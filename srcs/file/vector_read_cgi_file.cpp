
#include "vector_read_cgi_file.hpp"
#include "global.hpp"
#include <sys/types.h>
#include <sys/wait.h>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;

VectorReadCGIFile::VectorReadCGIFile(): max_buf_size(MAX_BUF), pid(ProcessID())
{
}

VectorReadCGIFile::VectorReadCGIFile(size_t max_buf_size, ProcessID const &pid) : max_buf_size(max_buf_size), pid(pid)
{
}

VectorReadCGIFile::~VectorReadCGIFile()
{
    DEBUG("VectorReadCGIFile() Destructor");
}

VectorReadCGIFile* VectorReadCGIFile::from_buf_size(size_t buf_size, ProcessID const &pid)
{
    DEBUG("VectorReadCGIFile::from_buf_size()");
    VectorReadCGIFile *file = new VectorReadCGIFile(buf_size, pid);
    return (file);
}


int VectorReadCGIFile::read(char **buf, size_t size)
{
    if(this->state == FILE_COMPLETED_READ){
        return (-1);
    }

    this->buffer.ref(buf, size);
    return (this->buffer.size());

}

int VectorReadCGIFile::write(char **buf, size_t size)
{
    size_t current_size = this->buffer.size();
    (this->buffer.append(*buf, size));
    size_t new_size = this->buffer.size();

    return (int)(new_size - current_size);
}

int VectorReadCGIFile::save(char *buf, size_t size)
{
    size_t current_size = this->buffer.size();
    this->buffer.append(buf, size);
    size_t new_size = this->buffer.size();
    return (int)(new_size - current_size);
}



size_t VectorReadCGIFile::size()
{
    return (this->buffer.size());
}

void VectorReadCGIFile::clear_read()
{
    this->state = FILE_OPEN;
}

bool VectorReadCGIFile::completed()
{
    return (true);
}

