#include "webserv_io.hpp"

WebservIO::WebservIO()
{
    ;
}

WebservIO::~WebservIO()
{
    ;
}


void WebservIO::set_source(File *file)
{
    this->source_ = file;
}

void WebservIO::set_destination(File *file)
{
    this->destination_ = file;
}

File *WebservIO::source()
{
    return (this->source_);
}

File *WebservIO::source_for_read()
{
    return (this->read_source_);
}

File *WebservIO::source_for_write()
{
    return (this->write_source_);
}

File *WebservIO::destination()
{
    return (this->destination_);
}

File *WebservIO::destination_for_read()
{
    return (this->read_destination_);
}

File *WebservIO::destination_for_write()
{
    return (this->write_destination_);
}

void WebservIO::set_write_io(File *src, File *dst)
{
    this->write_source_ = src;
    this->write_destination_ = dst;
}

void WebservIO::set_read_io(File *src, File *dst)
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
    DEBUG("WebservIO::switching_io");
    if(epoll_event == EPOLLIN){
        this->source_ = this->read_source_;
        this->destination_ = this->read_destination_;
        this->in_out_ = epoll_event;
    }else if(epoll_event == EPOLLOUT){
        this->source_ = this->write_source_;
        this->destination_ = this->write_destination_;
        this->in_out_ = epoll_event;
    }else{
        ERROR("neither EPOLLIN nor EPOLLOUT");
        throw std::runtime_error("neither EPOLLIN nor EPOLLOUT");
    }
}
