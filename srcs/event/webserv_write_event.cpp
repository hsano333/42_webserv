#include "webserv_write_event.hpp"
#include "socket_writer.hpp"
#include "opened_socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"


WebservWriteEvent::WebservWriteEvent()
{
    ;
}

WebservWriteEvent::WebservWriteEvent(
        //FileDiscriptor fd,
        File *src,
        File *dst
        //File *source
        //IWriter *writer
        )
    :
        //fd_(fd),
        source_file(src),
        destination_file(dst),
        //source_file(source),
        //destination_file(NULL),
        timeout_count_(0)
        //writer(writer)
{
    ;
}

WebservWriteEvent::~WebservWriteEvent()
{
    ;
}


void tmp_func(WebservWriteEvent *event, WebservEntity *entity)
{
    (void)event;
    (void)entity;
    // do nothing

}

WebservEvent *WebservWriteEvent::from_event(WebservEvent *event, File *src, File *dst)
{
    DEBUG("WebservWriteEvent::from_event()");
    //File *file = OpenedSocketFile::from_fd(writer, res->fd());
    WebservWriteEvent *write_event =  new WebservWriteEvent();
    WebservEvent *new_event =  new WebservEvent(write_event, tmp_func, event->entity());

    new_event->entity()->io().set_source(src);
    new_event->entity()->io().set_destination(dst);
    /*
    if(event->res()->get_file()){
            cout << "check file No.1" << endl;
            cout << "check file No.1" << endl;
            cout << "check file No.1" << endl;
    }else{
            cout << "check file not No.1" << endl;
            cout << "check file not No.1" << endl;
            cout << "check file not No.1" << endl;
    }
    */
    //write_event->destination_file = OpenedSocketFile::from_fd(writer, event->fd());

    return (new_event);
}


EWebservEvent WebservWriteEvent::which()
{
    return (WRITE_EVENT);
}


FileDiscriptor &WebservWriteEvent::fd()
{
    return (this->fd_);
}

Request *WebservWriteEvent::req()
{
    return (this->req_);
}

Response *WebservWriteEvent::res()
{
    return (this->res_);
}

File *WebservWriteEvent::src()
{
    return (this->source_file);
}

File *WebservWriteEvent::dst()
{
    return (this->destination_file);
}

void WebservWriteEvent::set_src(File *file)
{
    this->source_file = file;
}

void WebservWriteEvent::set_dst(File *file)
{
    this->destination_file = file;
}

/*
HttpData *WebservWriteEvent::source()
{
    return (this->source_);
}
*/

bool WebservWriteEvent::is_completed()
{
    return (this->is_completed_);
}
void WebservWriteEvent::set_completed(bool flag)
{
    this->is_completed_ = flag;
}


void WebservWriteEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservWriteEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservWriteEvent::timeout_count()
{
    return (this->timeout_count_);
}

//WebservEvent* WebservWriteEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
WebservEvent* WebservWriteEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservWriteEvent::make_next_event()");
    //return (event_factory->make_application_event(event));
    return (event_factory->make_clean_event(event, false));
}

E_EpollEvent WebservWriteEvent::get_next_epoll_event(WebservEvent *event)
{
    (void)event;
    DEBUG("WebservWriteEvent::get_next_epoll_event()");
    return (EPOLL_READ);
}


int WebservWriteEvent::write(char *buf, size_t size)
{
    return (this->dst()->write(&buf, size));
}


/*
WebservEntity*WebservWriteEvent::entity()
{
    return (this->entity_);
}

void WebservWriteEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservWriteEvent::cgi_event()
{
    return (this->cgi_event_);
}
*/

