#include "webserv_application_with_cgi_event.hpp"
#include "webserv_event.hpp"

WebservApplicationWithCgiEvent::WebservApplicationWithCgiEvent(
                            FileDiscriptor fd,
                            Request *req)
                            :
                            fd_(fd),
                            req_(req),
                            res_(NULL),
                            file_(NULL),
                            source_file(NULL),
                            destination_file(NULL),
                            timeout_count_(0),
                            is_completed_(false),
                            cgi_event_(NULL)
{

};

WebservApplicationWithCgiEvent::~WebservApplicationWithCgiEvent()
{
};

WebservApplicationWithCgiEvent *WebservApplicationWithCgiEvent::from_event(WebservEvent *event)
{
    return (new WebservApplicationWithCgiEvent(event->fd(), event->req()));
};

EWebservEvent WebservApplicationWithCgiEvent::which()
{
    return (APPLICATION_EVENT);
}

WebservEvent* WebservApplicationWithCgiEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservApplicationWithCgiEvent::make_next_event");
    (void)event_factory;
    //todo
    /*
    if(event->cgi_event()){
        cout << "test No.1 " << endl;
        WebservCgiEvent *cgi = event->cgi_event();
        cout << "test No.2 cgi=" << cgi << endl;
        cout << "test No.2 event->req()=" << event->req() << endl;
        //event_factory->make_and_push_write_cgi_event(cgi->pid(), cgi->fd_out(), event->req());
        event_factory->make_and_push_read_cgi_event(cgi->pid(), cgi->fd_in());
        MYINFO("cgi pid=" + Utility::to_string(cgi->pid()));
        MYINFO("cgi fd_in=" + cgi->fd_in().to_string());
        MYINFO("cgi fd_out=" + cgi->fd_out().to_string());
        cout << "test No.20" << endl;

        return (event_factory->make_nothing_event(cgi->pid(), cgi->pid()));
        //return (event_factory->make_write_event(event, event->res()));
    }
    */
    //return (event_factory->make_write_event(event, event->res()));
    //return (event_factory->make_making_response_event(event));
    return (event);
}

E_EpollEvent WebservApplicationWithCgiEvent::get_next_epoll_event()
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

FileDiscriptor WebservApplicationWithCgiEvent::fd()
{
    return (this->fd_);
}

Request *WebservApplicationWithCgiEvent::req()
{
    return (req_);
}

Response *WebservApplicationWithCgiEvent::res()
{
    return (this->res_);
}

File *WebservApplicationWithCgiEvent::src()
{
    return (this->source_file);
}

File *WebservApplicationWithCgiEvent::dst()
{
    return (this->destination_file);
}

void WebservApplicationWithCgiEvent::set_src(File *file)
{
    this->source_file = file;
}

void WebservApplicationWithCgiEvent::set_dst(File *file)
{
    this->destination_file = file;
}


bool WebservApplicationWithCgiEvent::is_completed()
{
    return (this->is_completed_);
}
void WebservApplicationWithCgiEvent::set_completed(bool flag)
{
    DEBUG("WebservApplicationWithCgiEvent::set_completed");
    this->is_completed_ = flag;
}

void WebservApplicationWithCgiEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservApplicationWithCgiEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservApplicationWithCgiEvent::timeout_count()
{
    return (this->timeout_count_);
}

void WebservApplicationWithCgiEvent::set_response(Response *res)
{
    this->res_ = res;
}


void WebservApplicationWithCgiEvent::set_file(File *file)
{
    this->file_ = file;
}

File *WebservApplicationWithCgiEvent::file()
{
    return (this->file_);
}

void WebservApplicationWithCgiEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    DEBUG("WebservApplicationWithCgiEvent::set_cgi_event()");
    cout << "copy cgi_event=" << cgi_event << endl;
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservApplicationWithCgiEvent::cgi_event()
{
    DEBUG("WebservApplicationWithCgiEvent::cgi_event()");
    return (this->cgi_event_);
}


ApplicationResult *WebservApplicationWithCgiEvent::result()
{
    return (this->result_);
}

void WebservApplicationWithCgiEvent::set_result(ApplicationResult *result)
{
    this->result_ = result;
}
