
#include "vector_file.hpp"
#include "global.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;

VectorFile::VectorFile(): max_buf_size(MAX_BUF)
{
    ;
}

VectorFile::VectorFile(size_t max_buf_size) : max_buf_size(max_buf_size)
{
    ;
}

VectorFile::~VectorFile()
{
}


VectorFile* VectorFile::from_ref(std::string const& buf_ref)
{
    DEBUG("VectorFile::from_ref()");
    VectorFile *file = new VectorFile(buf_ref.size());
    file->buf.resize(buf_ref.size());
    for(size_t i=0;i<buf_ref.size();i++){
        file->buf[i] = buf_ref[i];
    }
    return (file);
}

VectorFile* VectorFile::from_buf_size(size_t buf_size)
{
    DEBUG("VectorFile::from_buf_size()");
    
    //(void)buf_size;
    VectorFile *file = new VectorFile(buf_size);
    //file->buf = std::vector<char> new_buf(buf_size);
    return (file);
}

int VectorFile::open()
{
    this->state = FILE_OPEN;
    return 0;
}

int VectorFile::read(char **buf, size_t size)
{
    DEBUG("VectorFile::read() buf_size=" + Utility::to_string(this->buf.size()));
    if(this->state == FILE_COMPLETED_READ){
        return (-1);
    }
    // sizeは無視する(ポインタを渡すのでサイズの大小に関係がない)
    (void)size;
    this->buf.push_back('\0');

    //this->buf.push_back('\0');
    *buf = &(this->buf[0]);
    for(size_t i=0;i<this->buf.size();i++){
        printf("%c", this->buf[i]);
    }

    this->state = FILE_COMPLETED_READ;
    printf("vector file buffer=%s\n", *buf);

    return (this->buf.size());
}

int VectorFile::write(char **buf, size_t size)
{
    DEBUG("VectorFile::write() size=" + Utility::to_string(size));
    char *p_buf = *buf;
    for(size_t i=0; i<size;i++){
        this->buf.push_back(p_buf[i]);
    }
    //this->buf.push_back('\0');

    if(this->buf.size() > this->max_buf_size){
        WARNING("buf size=" + Utility::to_string(this->buf.size()));
        WARNING("max size=" + Utility::to_string(this->max_buf_size));
        // 指定したサイズを超過した際、-1を返すことで次の処理(パーサー処理)に移行する。
        // ステータスコードの超過なのか、あるいはbodyに入っているかどうかは
        // 次の処理で判定する。
        return (-1);
    }
    return (size);
}

int VectorFile::save(char *buf, size_t size)
{
    DEBUG("VectorFile::save()");
    for(size_t i=0;i<size;i++){
        this->buf.push_back(buf[i]);
    }
    if(this->buf.size() > max_buf_size){
        ERROR("exceed vecotor size");
        throw std::runtime_error("exceed vecotr size");
    }
    return (this->buf.size());
}

int VectorFile::close()
{
    this->state = FILE_CLOSE;
    return (0);
}

bool VectorFile::can_read()
{
    return (true);
}

size_t VectorFile::size()
{
    return (this->buf.size());
}

bool VectorFile::is_chunk()
{
    return (false);
}

int VectorFile::remove()
{
    // error file is not exist
    return (-1);
}

std::string const &VectorFile::path()
{
    return (path_);
}
