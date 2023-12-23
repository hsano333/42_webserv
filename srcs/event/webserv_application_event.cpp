#include "webserv_application_event.hpp"
#include "webserv_event.hpp"

WebservApplicationEvent::WebservApplicationEvent(
                            FileDiscriptor fd,
                            Request *req)
                            :
                            fd_(fd),
                            req_(req),
                            res_(NULL),
                            file_(NULL),
                            timeout_count_(0),
                            is_completed_(false),
                            cgi_event_(NULL)
{

};

WebservApplicationEvent::~WebservApplicationEvent()
{
};

WebservApplicationEvent *WebservApplicationEvent::from_event(WebservEvent *event)
{
    return (new WebservApplicationEvent(event->fd(), event->req()));
};

EWebservEvent WebservApplicationEvent::which()
{
    return (APPLICATION_EVENT);
}

WebservEvent* WebservApplicationEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservApplicationEvent::make_next_event");
    //todo
    if(event->cgi_event()){
        cout << "test No.1 " << endl;
        WebservCgiEvent *cgi = event->cgi_event();
        cout << "test No.2 cgi=" << cgi << endl;
        cout << "test No.2 event->req()=" << event->req() << endl;
        event_factory->make_cgi_event(cgi->pid(), cgi->fd_in(), cgi->fd_out(), event->req());
        MYINFO("cgi pid=" + Utility::to_string(cgi->pid()));
        MYINFO("cgi fd_in=" + cgi->fd_in().to_string());
        MYINFO("cgi fd_out=" + cgi->fd_out().to_string());
        cout << "test No.20" << endl;
        return (event_factory->make_write_event_for_cgi(event, event->res()));
    }
    return (event_factory->make_write_event(event, event->res()));
}

E_EpollEvent WebservApplicationEvent::get_next_epoll_event()
{
    if (this->is_completed_){
        if(this->cgi_event() == NULL){
            return (EPOLL_WRITE);
        }else{
            return (EPOLL_CGI_STOP);
        }
    }else{
        return (EPOLL_READ);
    }
}

FileDiscriptor WebservApplicationEvent::fd()
{
    return (this->fd_);
}

Request *WebservApplicationEvent::req()
{
    return (req_);
}

Response *WebservApplicationEvent::res()
{
    return (this->res_);
}

File *WebservApplicationEvent::src()
{
    return (this->source_file);
}

File *WebservApplicationEvent::dst()
{
    return (this->destination_file);
}


bool WebservApplicationEvent::is_completed()
{
    return (this->is_completed_);
}
void WebservApplicationEvent::set_completed(bool flag)
{
    DEBUG("WebservApplicationEvent::set_completed");
    this->is_completed_ = flag;
}

void WebservApplicationEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservApplicationEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservApplicationEvent::timeout_count()
{
    return (this->timeout_count_);
}

void WebservApplicationEvent::set_response(Response *res)
{
    this->res_ = res;
}


void WebservApplicationEvent::set_file(File *file)
{
    this->file_ = file;
}

File *WebservApplicationEvent::file()
{
    return (this->file_);
}

void WebservApplicationEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    DEBUG("WebservApplicationEvent::set_cgi_event()");
    cout << "copy cgi_event=" << cgi_event << endl;
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservApplicationEvent::cgi_event()
{
    DEBUG("WebservApplicationEvent::cgi_event()");
    return (this->cgi_event_);
}
