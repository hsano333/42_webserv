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
    DEBUG("BufferController::copy size:" + Utility::to_string(this->buf.size()));
}

int BufferController::save(char *data, size_t size)
{
    DEBUG("BufferController::save");
    this->buf.resize(size);
    Utility::memcpy(&(this->buf[0]), data, size);
    return (this->buf.size());

}

int BufferController::retrieve(char **data, size_t max_size)
{
    DEBUG("BufferController::retrieve buffer size=" + Utility::to_string(this->buf.size()));
    if(this->buf.size() <= 0){
        return (0);
    }
    DEBUG("test No.1");
    if(this->buf.size() > max_size)
    {
    DEBUG("test No.2");
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
    DEBUG("test No.3");
        for(size_t i=0;i<this->buf.size();i++){
            (*data)[i] = this->buf[i];
        }
        int return_size = buf.size();
    DEBUG("test No.5");
        this->buf.clear();
    DEBUG("test No.6");
        return return_size;
    }
}
