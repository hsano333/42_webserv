#include "webserv_io.hpp"
#include "http_exception.hpp"

WebservIO::WebservIO() : source_(NULL), destination_(NULL), read_source_(NULL), read_destination_(NULL), write_source_(NULL), write_destination_(NULL), is_read_completed_(false), is_write_completed_(false), total_write_size_(0), cgi_divided_(false), is_cgi_read_(true)
{
    DEBUG("WebservIO() Constructor");
}

WebservIO::~WebservIO()
{
    DEBUG("WebservIO() Destructor");
}


void WebservIO::set_source(WebservFile *file)
{
    DEBUG("WebservIO::set_source");
    this->source_ = file;
}

void WebservIO::set_destination(WebservFile *file)
{
    DEBUG("WebservIO::set_destination");
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


int WebservIO::save(char *data, size_t offset, size_t size)
{

    if(offset > size || size - offset > MAX_BUF){
        ERROR("offset > size=" + Utility::to_string(size) + ",offset=" + Utility::to_string(offset));
        throw HttpException("500");
    }

    this->tmp_buf.resize(size-offset);
    for(size_t i=0;i<size-offset;i++){
        this->tmp_buf[i] = data[i + offset];
    }
    return (this->tmp_buf.size());
}

int WebservIO::save(char const *data, size_t offset, size_t size)
{
    this->tmp_buf.resize(size-offset);
    for(size_t i=0;i<size-offset;i++){
        this->tmp_buf[i] = data[i + offset];
    }
    return (this->tmp_buf.size());
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

bool WebservIO::is_read_completed()
{
    return (this->is_read_completed_);
}

void WebservIO::set_read_completed(bool flag)
{
    this->is_read_completed_ = flag;
}

bool WebservIO::is_write_completed()
{
    return (this->is_write_completed_);
}

void WebservIO::set_write_completed(bool flag)
{
    this->is_write_completed_ = flag;
}

size_t WebservIO::total_status_header_size()
{

    DEBUG("WebservIO::total_status_header_size:" + Utility::to_string(this->total_status_header_size_));
    //DEBUG("WebservIO::total_write_size:" + Utility::to_string(&this->total_write_size_));
    //DEBUG("WebservIO::total_write_size:" + Utility::to_string(this));
    return (this->total_status_header_size_);
}

void WebservIO::set_total_status_header_size(size_t size)
{
    DEBUG("WebservIO::set_total_status_header_size:" + Utility::to_string(size));
    this->total_status_header_size_ = size;
}


size_t WebservIO::total_write_size()
{

    DEBUG("WebservIO::total_write_size:" + Utility::to_string(this->total_write_size_));
    //DEBUG("WebservIO::total_write_size:" + Utility::to_string(&this->total_write_size_));
    //DEBUG("WebservIO::total_write_size:" + Utility::to_string(this));
    return (this->total_write_size_);
}

void WebservIO::set_total_write_size(size_t size)
{
    DEBUG("WebservIO::set_total_write_size:" + Utility::to_string(size));
    this->total_write_size_ = size;
}

void WebservIO::add_total_write_size(size_t size)
{

    DEBUG("WebservIO::size:" + Utility::to_string(size));
    if(this->total_write_size_ >= this->total_write_size_ + size){
        ERROR("exceed total write buffer size");
        throw std::runtime_error("exceed total write buffer size");
    }
    this->total_write_size_ += size;
    DEBUG("WebservIO::total_write_size_:" + Utility::to_string(this->total_write_size_));
}


void WebservIO::set_cgi_divided(bool flag)
{
    this->cgi_divided_ = flag;
}

bool WebservIO::is_cgi_read()
{
    return (this->is_cgi_read_);
}

void WebservIO::set_is_cgi_read(bool flag)
{
    this->is_cgi_read_ = flag;
}

bool WebservIO::cgi_divided()
{
    return (this->cgi_divided_);
}
