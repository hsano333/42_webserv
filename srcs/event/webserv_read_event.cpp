#include "webserv_read_event.hpp"
#include "socket_reader.hpp"
#include "opened_socket_file.hpp"
#include "normal_reader.hpp"
#include "global.hpp"
#include "request_file.hpp"

WebservReadEvent::WebservReadEvent()
                                        :
                                        req_(NULL),
                                        res_(NULL),
                                        source_file(NULL),
                                        destination_file(NULL),
                                        fd_(FileDiscriptor::from_int(0)),
                                        //event_type(READ_EVENT),
                                        timeout_count_(0),
                                        is_completed_(false),
                                        reader(NULL)
{
}

/*
WebservReadEvent::WebservReadEvent(FileDiscriptor fd)
    :
        //req_(new Request()),
        req_(NULL),
        fd_(fd),
        timeout_count_(0)
        ireader(NULL)
{
    std::cout << "WebservReadEvent Constructor" << std::endl;

    ;
}
*/

WebservReadEvent::WebservReadEvent(FileDiscriptor fd, FileDiscriptor sockfd, IReader *reader)
    :
        //req_(new Request()),
        req_(NULL),
        res_(NULL),
        source_file(NULL),
        destination_file(NULL),
        fd_(fd),
        sockfd_(sockfd),
        timeout_count_(0),
        reader(reader)
{

    std::cout << "WebservReadEvent Constructor" << std::endl;
    //File *file = OpenedSocketFile::from_fd(reader, fd);
    //this->file = file;
}



WebservReadEvent::~WebservReadEvent()
{
    //delete ireader;
}

WebservReadEvent *WebservReadEvent::from_fd(FileDiscriptor fd, FileDiscriptor sockfd, IReader *reader)
{
    (void)sockfd;
    DEBUG("WebservReadEvent::from_fd() fd:" + fd.to_string());
    WebservReadEvent *event = new WebservReadEvent(fd, sockfd, reader);
    event->source_file = OpenedSocketFile::from_fd(reader, fd);
    event->destination_file = RequestFile::from_buf_size(MAX_BUF);

    return (event);
}


WebservReadEvent *WebservReadEvent::from_cgi_fd(FileDiscriptor fd, FileDiscriptor pid, IReader *reader)
{
    WebservReadEvent *event = new WebservReadEvent(fd, pid, reader);
    event->source_file = OpenedSocketFile::from_fd(reader, fd);
    //req->set_source_file(file);
    return (event);
}

WebservReadEvent *WebservReadEvent::from_event(WebservEvent *event, FileDiscriptor sockfd, IReader *reader)
{
    DEBUG("WebservReadEvent::from_event()");
    WebservReadEvent *new_event = new WebservReadEvent(event->fd(), sockfd, reader);
    DEBUG("WebservReadEvent::from_event() No.1");
    //if (event->req()){
        //delete event->req();
    //}
    if (event->res()){
        DEBUG("WebservReadEvent::from_event() No.11");
        delete event->res();
    }
    DEBUG("WebservReadEvent::from_event() No.2");
    Request *req;
    if(event->req()){
        req = event->req();
    }else{
        //FileDiscriptor sockfd = fd_manager->get_sockfd(event->fd());
        req = Request::from_fd(event->fd());
        //File *file = OpenedSocketFile::from_fd(reader, event->fd());
        //new_event->source_file = OpenedSocketFile::from_fd(reader, event->fd());
        //req->set_source_file(file);
    }
    DEBUG("WebservReadEvent::from_event() No.3");
    new_event->req_ = req;
    new_event->res_ = NULL;
    new_event->source_file = req;
    DEBUG("WebservReadEvent::from_event() No.4 make event:" + Utility::to_string(new_event));
    return (new_event);
}

EWebservEvent WebservReadEvent::which()
{
    return (READ_EVENT);
}


FileDiscriptor WebservReadEvent::fd()
{
    return (this->fd_);
}

Request *WebservReadEvent::req()
{
    return (req_);
}

Response *WebservReadEvent::res()
{
    return (NULL);
}

File *WebservReadEvent::src()
{
    return (this->source_file);
}

File *WebservReadEvent::dst()
{
    return (this->destination_file);
}

bool WebservReadEvent::is_completed()
{
    return (this->is_completed_);
}
void WebservReadEvent::set_completed(bool flag)
{
    this->is_completed_ = flag;
}


void WebservReadEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservReadEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservReadEvent::timeout_count()
{
    return (this->timeout_count_);
}

WebservEvent* WebservReadEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservReadEvent::make_next_event()");
    return (event_factory->make_making_request_event(event));
    //return (event_factory->make_application_event(event));
}

E_EpollEvent WebservReadEvent::get_next_epoll_event()
{
    if (this->is_completed_){
        return (EPOLL_NONE);
    }else{
        return (EPOLL_READ);
    }
}


    /*
#include <stdio.h>
int WebservReadEvent::read(char *buf, size_t size)
{

    MYINFO("WebservReadEvent::read() fd:" + Utility::to_string(this->fd_));
    if( this->reader == NULL){
        std::cout << "buf is NULL"  << std::endl;
    }
    printf("buf=%p", buf);
    ssize_t read_size = this->reader->read(this->fd_, buf, size, NULL);

    if(read_size < 0){
        MYINFO("read < 0:" + Utility::to_string(read_size));
        event->set_completed(true);
        return (-1);
    }else if(read_size == 0){
        ERROR("Read Connection Error");
        throw ConnectionException("Read Error");
    }
}

    */
void WebservReadEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservReadEvent::cgi_event()
{
    return (this->cgi_event_);
}

/*
IReader *WebservReadEvent::reader()
{
    return (this->reader);
}

void WebservReadEvent::read(char *buf, size_t size)
{

    int space = this->raw_buf_space();
    char* buf = this->get_raw_buf_pointer();
    FileDiscriptor fd = read_event->fd();
    size_t sum = this->buf_size();
    MYINFO("Receiver  fd:" + Utility::to_string(fd.to_int()));
    MYINFO("Receiver  sum:" + Utility::to_string(sum));
    MYINFO("Receiver  space:" + Utility::to_string(space));
    //space = 100;
    event->set_completed(false);
    while(1){
        char* p_data = &(buf[sum]);
        ssize_t tmp = source->read(&p_data, MAX_READ_SIZE);

        MYINFO("Receiver read < 0:" + Utility::to_string(tmp));
        if(tmp < 0){
            MYINFO("Receiver read < 0:" + Utility::to_string(tmp));
            event->set_completed(true);
            break;
        }else if(tmp == 0){
            ERROR("Read Connection Error");
            //source->close();
            throw ConnectionException("Read Error");
        }
        sum += tmp;
        space -= tmp;
        if(space <= 0){
            this->set_read_completed(false);
            break;
        }
    }
    //source->close();
    MYINFO("Receive data size::" + Utility::to_string(sum));
    cout << "read sum:" << sum << endl;
    cout << "read space:" << space << endl;
    buf[sum] = '\0';
    cout << "read buf:[" << buf << "]" << endl;
    this->set_buf_pos(sum);
    //this->decrement_raw_buf_size(sum);
    if (sum == 0){
        ERROR("Request data is zero");
        throw HttpException("400");
    }


    char *body_p = Utility::strnstr(this->buf(), "\r\n\r\n", (size_t)this->buf_size());

    if(body_p == NULL && this->buf_size() >= MAX_READ_SIZE){
        body_p = Utility::strnstr(this->buf(), "\r\n",(size_t)this->buf_size());
        if(body_p){
            ERROR("Invalid Request. Heade is too long ");
            throw HttpException("431");
        }
        ERROR("Invalid Request.  request line is too long ");
        throw HttpException("414");
    }
    //cout << "req->buf_size():" << req->buf_size() << endl;
    //char *buf_p = req->buf();
    //buf_p[req->buf_size()] = '\0';
    //printf("buf_p=%s\n", buf_p);
    if(body_p == NULL){
        DEBUG("WebservParser:: not still read Request header");
        event->set_completed(false);
        return ;
    }

    // bodyの開始位置の記録
    body_p += 4;
    this->set_buf_body(body_p, this->buf_size() - (size_t)(body_p - this->buf()));

    // parser処理でbodyを読み込まないため
    body_p -= 4;
    *body_p = '\0';


    event->set_completed(true);
}

int WebservReceiver::raw_buf_space()
{
    DEBUG("Request::raw_buf_space()  raw_buf_pos_:" + Utility::to_string(this->raw_buf_pos_));
    return (MAX_BUF - 1 - this->raw_buf_pos_);
}

size_t WebservReceiver::buf_size()
{
    return (this->raw_buf_pos_);
}

char *WebservReceiver::get_raw_buf_pointer()
{
    DEBUG("Request::get_raw_buf_pointer()  raw_buf_point:" + Utility::to_string(this->raw_buf_pos_));
    return &(this->buf_[this->raw_buf_pos_]);
}

void Request::WebservReceiver(size_t pos)
{
    DEBUG("Request::set_buf_pos():" + Utility::to_string(pos));
    this->raw_buf_pos_ = pos;
    //DEBUG("Request::raw_buf_space()  raw_buf_rest_size_:" + Utility::to_string(this->raw_buf_rest_size_));
    //return (raw_buf_pos_);
}
*/


