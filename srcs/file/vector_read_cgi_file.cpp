
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
    //this->buf.resize(max_buf_size);
}

VectorReadCGIFile::VectorReadCGIFile(size_t max_buf_size, ProcessID const &pid) : max_buf_size(max_buf_size), pid(pid)
{
    //this->buf.resize(max_buf_size);
}

VectorReadCGIFile::~VectorReadCGIFile()
{
    DEBUG("VectorReadCGIFile() Destructor");
}

/*
VectorReadCGIFile* VectorReadCGIFile::from_buf(char *buf, size_t size)
{
    DEBUG("VectorReadCGIFile::from_buf()");
    VectorReadCGIFile *file = new VectorReadCGIFile(size);
    file->buffer.save(buf, size);
    return (file);
}
*/

/*
VectorReadCGIFile* VectorReadCGIFile::from_ref(std::string const& buf_ref)
{
    DEBUG("VectorReadCGIFile::from_ref() size:" + Utility::to_string(buf_ref.size()));
    VectorReadCGIFile *file = new VectorReadCGIFile(buf_ref.size());
    DEBUG("VectorReadCGIFile::from_ref() No.1");

    file->buffer.save(&(buf_ref[0]), buf_ref.size());

    DEBUG("VectorReadCGIFile::from_ref() No.2");
    //file->index = buf_ref.size();
    return (file);
}
*/

VectorReadCGIFile* VectorReadCGIFile::from_buf_size(size_t buf_size, ProcessID const &pid)
{
    DEBUG("VectorReadCGIFile::from_buf_size()");
    //(void)buf_size;
    VectorReadCGIFile *file = new VectorReadCGIFile(buf_size, pid);
    //file->buf = std::vector<char> new_buf(buf_size);
    return (file);
}


int VectorReadCGIFile::read(char **buf, size_t size)
{
    DEBUG("vector file test No.1");
    if(this->state == FILE_COMPLETED_READ){
        DEBUG("vector file test No.2");
        return (-1);
    }
    //return (this->reader(buf, size));

    DEBUG("VectorReadCGIFile::read() buf_size=" + Utility::to_string(this->buffer.size()));
        DEBUG("vector file test No.3");
    // sizeは無視する(ポインタを渡すのでサイズの大小に関係がない)
    (void)size;
    //this->buf.push_back('\0');
        DEBUG("vector file test No.4");

    //this->buf.push_back('\0');
    //this->buf[this->buf.size()] = '\0';
    //*buf = &(this->buffer[0]);
    this->buffer.ref(buf, size);
    return (this->buffer.size());

    /*
    printf("\n\nvector_file test[");
        DEBUG("vector file test No.5");
    for(size_t i=0;i<this->buf.size();i++){
        printf("%c", this->buf[i]);
    }
    printf("]\n\n");
    //buf[this->buf.size()-1] = '\0';

    //string tmp = *buf;
        //DEBUG("vector file test No.6 buf=" + tmp);
    this->state = FILE_COMPLETED_READ;
    printf("vector file buffer=%s\n", *buf);

    return (this->buf.size());
    */
}

int VectorReadCGIFile::write(char **buf, size_t size)
{
    DEBUG("VectorReadCGIFile::write() size=" + Utility::to_string(size));
    size_t current_size = this->buffer.size();
    (this->buffer.append(*buf, size));
    size_t new_size = this->buffer.size();

    return (int)(new_size - current_size);

    /*
    size_t buf_size = this->buffer.size();
    if(size + buf_size > this->buf.size()){
        ERROR("VectorReadCGIFile: write exceed");
        throw std::runtime_error("VectorReadCGIFile: write exceed");
    }
    char *p_buf = *buf;
    p_buf[size] = '\0';
    DEBUG("VectorReadCGIFile::write() buf=" + Utility::to_string(p_buf));
    //size_t 
    //size_t rest = this->max_buf_size - this->index - 1;
    //size_t min = Utility::min(size, rest);
    for(size_t i=0; i<size;i++){
        //DEBUG("VectorReadCGIFile::write() i=" + Utility::to_string(i));
        //DEBUG("VectorReadCGIFile::write() this->index=" + Utility::to_string(this->index));
        this->buf[buf_size + i] = p_buf[i];
    }
    //DEBUG("VectorReadCGIFile::write()min =" + Utility::to_string(min));
    //DEBUG("VectorReadCGIFile::write()rest=" + Utility::to_string(rest));
    return (size);
    */

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

int VectorReadCGIFile::save(char *buf, size_t size)
{
    DEBUG("VectorReadCGIFile::save()");
    size_t current_size = this->buffer.size();
    this->buffer.append(buf, size);
    size_t new_size = this->buffer.size();
    return (int)(new_size - current_size);
    /*
    for(size_t i=0;i<size;i++){
        this->buf.push_back(buf[i]);
    }
    if(this->buf.size() > max_buf_size){
        ERROR("exceed vecotor size");
        throw std::runtime_error("exceed vecotr size");
    }
    return (this->buf.size());
    */
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

    /*
    int wstatus = 0;
    waitpid(this->pid.to_int(), &wstatus,  WUNTRACED | WCONTINUED | WNOHANG);

    if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) != 0) {
        MYINFO("VectorReadCGIFile::completed True");
        return (true);
    }
    MYINFO("VectorReadCGIFile::completed False pid:" + pid.to_string());
    */

    return (true);
}

/*
size_t VectorReadCGIFile::change_reader(IReader *reader)
{
    this->reader = reader;
}
*/

/*
bool VectorReadCGIFile::is_chunk()
{
    return (false);
}
*/


