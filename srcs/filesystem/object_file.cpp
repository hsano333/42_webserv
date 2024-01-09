
#include "object_file.hpp"
#include "global.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;

ObjectFile::ObjectFile(): max_buf_size(0)
{
    ;
}

ObjectFile::ObjectFile(size_t max_buf_size) : max_buf_size(max_buf_size)
{
    ;
}

ObjectFile::~ObjectFile()
{
}

ObjectFile* ObjectFile::from_buf_size(size_t buf_size)
{
    
    //(void)buf_size;
    ObjectFile *file = new ObjectFile(buf_size);
    //file->buf = std::vector<char> new_buf(buf_size);
    return (file);
}

int ObjectFile::open()
{
    this->state = FILE_OPEN;
    return 0;
}

int ObjectFile::read(char **buf, size_t size)
{
    (void)size;
    if (this->state != FILE_OPEN){
        return (0);
    }
    std::cout << "ObjectFile::read No.3" << std::endl;
    *buf = const_cast<char*>(&(this->text[0]));
    std::cout << "ObjectFile::read No.4:" << this->text << std::endl;

    this->state = FILE_CLOSE;
    return this->text.size();
}

int ObjectFile::write(char **buf, size_t size)
{
    char *p_buf = *buf;
    for(size_t i=0; i<size;i++){
        this->buf.push_back(p_buf[i]);
    }

    if(this->buf.size() >= this->max_buf_size){
        throw std::runtime_error("buf size is exceed");
    }
    return (0);
}

int ObjectFile::close()
{
    this->state = FILE_CLOSE;
    return (0);
}

bool ObjectFile::can_read()
{
    return (true);
}

size_t ObjectFile::size()
{
    return (this->text.size());
}

bool ObjectFile::is_chunk()
{
    return (false);
}


int ObjectFile::remove()
{
    // error file is not exist
    return (-1);
}

std::string const &ObjectFile::path()
{
    return (this->text);
}
