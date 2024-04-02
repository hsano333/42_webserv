
#include "vector_file.hpp"
#include "global.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;

VectorFile::VectorFile(): max_buf_size(MAX_BUF)
{
    //this->buf.resize(max_buf_size);
}

VectorFile::VectorFile(size_t max_buf_size) : max_buf_size(max_buf_size)
{
    //this->buf.resize(max_buf_size);
}

VectorFile::~VectorFile()
{
    DEBUG("VectorFile() Destructor");
}

VectorFile* VectorFile::from_buf(char *buf, size_t size)
{
    DEBUG("VectorFile::from_buf()");
    VectorFile *file = new VectorFile(size);
    file->buf.resize(size);
    for(size_t i=0;i<size;i++){
        file->buf[i] = buf[i];
    }
    //file->index = size;
    return (file);
}

VectorFile* VectorFile::from_ref(std::string const& buf_ref)
{
    DEBUG("VectorFile::from_ref()");
    VectorFile *file = new VectorFile(buf_ref.size());
    //file->buf.resize(buf_ref.size());
    for(size_t i=0;i<buf_ref.size();i++){
        file->buf[i] = buf_ref[i];
    }
    //file->index = buf_ref.size();
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


int VectorFile::read(char **buf, size_t size)
{

    DEBUG("vector file test No.1");
    if(this->state == FILE_COMPLETED_READ){
        DEBUG("vector file test No.2");
        return (-1);
    }
    //return (this->reader(buf, size));

    DEBUG("VectorFile::read() buf_size=" + Utility::to_string(this->buf.size()));
        DEBUG("vector file test No.3");
    // sizeは無視する(ポインタを渡すのでサイズの大小に関係がない)
    (void)size;
    //this->buf.push_back('\0');
        DEBUG("vector file test No.4");

    //this->buf.push_back('\0');
    //this->buf[this->buf.size()] = '\0';
    *buf = &(this->buf[0]);
        DEBUG("vector file test No.5");
    for(size_t i=0;i<this->buf.size();i++){
        printf("%c", this->buf[i]);
    }

        DEBUG("vector file test No.6");
    this->state = FILE_COMPLETED_READ;
    printf("vector file buffer=%s\n", *buf);

    return (this->buf.size());
}

int VectorFile::write(char **buf, size_t size)
{
    DEBUG("VectorFile::write() size=" + Utility::to_string(size));
    size_t buf_size = this->buf.size();
    this->buf.resize(buf_size + size);
    char *p_buf = *buf;
    //size_t 
    //size_t rest = this->max_buf_size - this->index - 1;
    //size_t min = Utility::min(size, rest);
    for(size_t i=0; i<size;i++){
        //DEBUG("VectorFile::write() i=" + Utility::to_string(i));
        //DEBUG("VectorFile::write() this->index=" + Utility::to_string(this->index));
        this->buf[buf_size + i] = p_buf[i];
    }
    //DEBUG("VectorFile::write()min =" + Utility::to_string(min));
    //DEBUG("VectorFile::write()rest=" + Utility::to_string(rest));
    return (size);

    //if(min != size){
        //return (this->max_buf_size-this->index);
        //WARNING("buf size=" + Utility::to_string(this->buf.size()));
        //WARNING("max size=" + Utility::to_string(this->max_buf_size));
        // 指定したサイズを超過した際、-1を返すことで次の処理(パーサー処理)に移行する。
        // ステータスコードの超過なのか、あるいはbodyに入っているかどうかは
        // 次の処理で判定する。
        //return (-1);
    //}
    //return (size);
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



size_t VectorFile::size()
{
    return (this->buf.size());
}

void VectorFile::clear_read()
{
    this->state = FILE_OPEN;
}

/*
size_t VectorFile::change_reader(IReader *reader)
{
    this->reader = reader;
}
*/

/*
bool VectorFile::is_chunk()
{
    return (false);
}
*/


