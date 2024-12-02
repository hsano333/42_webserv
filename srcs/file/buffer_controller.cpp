#include "buffer_controller.hpp"
#include "utility.hpp"
#include "global.hpp"



BufferController::BufferController()
{
;
}

BufferController::~BufferController()
{
;
}

void BufferController::copy(std::vector<char> &buf)
{
    DEBUG("BufferController::copy size:" + Utility::to_string(buf.size()));
    this->buf = buf;
}

int BufferController::save(char *data, size_t size)
{
    DEBUG("BufferController::save");
    this->buf.resize(size);
    Utility::memcpy(&(this->buf[0]), data, size);
    return (this->buf.size());

}

int BufferController::save(char const *data, size_t size)
{
    this->save(const_cast<char *>(data), size);
    return (this->buf.size());
}

void BufferController::ref(char **data, size_t size)
{
    (void)size;
    *data = &(this->buf[0]);
}

size_t BufferController::append(char const *data, size_t size)
{
    for(size_t i=0;i<size;i++){
        this->buf.push_back(data[i]);
    }
    return (this->buf.size());
}

int BufferController::retrieve(char **data, size_t max_size)
{
    DEBUG("BufferController::retrieve buffer size=" + Utility::to_string(this->buf.size()));
    if(this->buf.size() <= 0){
        return (0);
    }
    if(this->buf.size() > max_size)
    {
        for(size_t i=0; i<max_size; i++){
            (*data)[i] = this->buf[i];
        }
        this->buf.clear();
        std::vector<char> tmp(this->buf.size() - max_size);
        for(size_t i=max_size; i<this->buf.size(); i++){
            tmp[i] = this->buf[i+max_size];
        }
        this->buf = tmp;
        return (max_size);
    }
    else
    {
        for(size_t i=0;i<this->buf.size();i++){
            (*data)[i] = this->buf[i];
        }
        int return_size = buf.size();
        this->buf.clear();
        return return_size;
    }
}

size_t BufferController::size() const
{
    return (this->buf.size());
}
