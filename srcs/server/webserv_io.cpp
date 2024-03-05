#include "webserv_io.hpp"

WebservIO::WebservIO() : source_(NULL), destination_(NULL), read_source_(NULL), read_destination_(NULL), write_source_(NULL), write_destination_(NULL)
{
    ;
}

WebservIO::~WebservIO()
{
    ;
}


void WebservIO::set_source(WebservFile *file)
{
    this->source_ = file;
}

void WebservIO::set_destination(WebservFile *file)
{
    this->destination_ = file;
}

WebservFile *WebservIO::source()
{
    return (this->source_);
}

WebservFile *WebservIO::source_for_read()
{
    return (this->read_source_);
}

WebservFile *WebservIO::source_for_write()
{
    return (this->write_source_);
}

WebservFile *WebservIO::destination()
{
    return (this->destination_);
}

WebservFile *WebservIO::destination_for_read()
{
    return (this->read_destination_);
}

WebservFile *WebservIO::destination_for_write()
{
    return (this->write_destination_);
}

void WebservIO::set_write_io(WebservFile *src, WebservFile *dst)
{
    this->write_source_ = src;
    this->write_destination_ = dst;
}

void WebservIO::set_read_io(WebservFile *src, WebservFile *dst)
{
    this->read_source_ = src;
    this->read_destination_ = dst;
}

void WebservIO::set_write_fd(FileDiscriptor const &fd)
{
    this->write_fd_ = fd;
}

void WebservIO::set_read_fd(FileDiscriptor const &fd)
{
    this->read_fd_ = fd;
}

void WebservIO::set_write_fd(FileDiscriptor &fd)
{
    this->write_fd_ = fd;
}

void WebservIO::set_read_fd(FileDiscriptor &fd)
{
    this->read_fd_ = fd;
}

FileDiscriptor  &WebservIO::get_write_fd()
{
    return (this->write_fd_);
}

FileDiscriptor  &WebservIO::get_read_fd()
{
    return (this->read_fd_);
}

void WebservIO::set_in_out(uint32_t inout)
{
    this->in_out_ = inout;
}

uint32_t WebservIO::in_out()
{
    return (this->in_out_);
}

void WebservIO::switching_io(uint32_t epoll_event)
{
    if(epoll_event == EPOLLIN){
        DEBUG("WebservIO::switching_io EPOLL_IN");
        this->source_ = this->read_source_;
        this->destination_ = this->read_destination_;
        this->in_out_ = epoll_event;
    }else if(epoll_event == EPOLLOUT){
        DEBUG("WebservIO::switching_io EPOLL_OUT");
        this->source_ = this->write_source_;
        this->destination_ = this->write_destination_;
        this->in_out_ = epoll_event;
    }else{
        ERROR("neither EPOLLIN nor EPOLLOUT");
        throw std::runtime_error("neither EPOLLIN nor EPOLLOUT");
    }
}


int WebservIO::save(char *data, size_t size)
{
    this->tmp_buf.resize(size);
    for(size_t i=0;i<size;i++){
        this->tmp_buf[i] = data[i];
    }
    return this->tmp_buf.size();
}

size_t WebservIO::load(char **data)
{
    *data = &(this->tmp_buf[0]);
    return (this->tmp_buf.size());

}

void WebservIO::clear_tmp_data()
{
    this->tmp_buf.clear();
}

