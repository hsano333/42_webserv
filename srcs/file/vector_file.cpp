
#include "vector_file.hpp"
#include "global.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;

VectorFile::VectorFile(): max_buf_size(MAX_BUF)
{
}

VectorFile::VectorFile(size_t max_buf_size) : max_buf_size(max_buf_size)
{
}

VectorFile::~VectorFile()
{
    DEBUG("VectorFile() Destructor");
}

VectorFile* VectorFile::from_buf(char *buf, size_t size)
{
    DEBUG("VectorFile::from_buf()");
    VectorFile *file = new VectorFile(size);
    file->buffer.save(buf, size);
    return (file);
}

VectorFile* VectorFile::from_ref(std::string const& buf_ref)
{
    DEBUG("VectorFile::from_ref() size:" + Utility::to_string(buf_ref.size()));
    VectorFile *file = new VectorFile(buf_ref.size());

    file->buffer.save(&(buf_ref[0]), buf_ref.size());
    return (file);
}

VectorFile* VectorFile::from_buf_size(size_t buf_size)
{
    DEBUG("VectorFile::from_buf_size()");

    VectorFile *file = new VectorFile(buf_size);
    return (file);
}


int VectorFile::read(char **buf, size_t size)
{
    if(this->state == FILE_COMPLETED_READ){
        return (-1);
    }

    (void)size;
    this->buffer.ref(buf, size);
    return (this->buffer.size());

}

int VectorFile::write(char **buf, size_t size)
{
    size_t current_size = this->buffer.size();
    (this->buffer.append(*buf, size));
    size_t new_size = this->buffer.size();

    return (int)(new_size - current_size);
}

int VectorFile::save(char *buf, size_t size)
{
    DEBUG("VectorFile::save()");
    size_t current_size = this->buffer.size();
    this->buffer.append(buf, size);
    size_t new_size = this->buffer.size();
    return (int)(new_size - current_size);
}



size_t VectorFile::size()
{
    return (this->buffer.size());
}

void VectorFile::clear_read()
{
    this->state = FILE_OPEN;
}

