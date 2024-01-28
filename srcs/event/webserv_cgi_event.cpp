#include "webserv_cgi_event.hpp"
#include "global.hpp"

WebservCgiEvent::WebservCgiEvent() : cgi_triger_(0), is_cgi_(false), is_completed_(false)
{
;
}

WebservCgiEvent::~WebservCgiEvent()
{
;
}

WebservCgiEvent::WebservCgiEvent(WebservCgiEvent const &cgi_event)
{
    DEBUG("WebservCgiEvent::WebservCgiEvent(): copy constructor");
    *this = cgi_event;
}

WebservCgiEvent& WebservCgiEvent::operator=(WebservCgiEvent const &cgi_event)
{
    DEBUG("WebservCgiEvent::WebservCgiEvent(): copy assignment ");
    if(this == &cgi_event){
        return (*this);
    }
    this->fd_in_ = cgi_event.fd_in_;
    this->fd_out_ = cgi_event.fd_out_;

    this->cgi_triger_ = cgi_event.cgi_triger_;
    this->is_cgi_ = cgi_event.is_cgi_;
    return (*this);
}


void WebservCgiEvent::set_fd_in(FileDiscriptor fd)
{
    this->fd_in_ = fd;
}

void WebservCgiEvent::set_fd_out(FileDiscriptor fd)
{
    this->fd_out_ = fd;
}

FileDiscriptor WebservCgiEvent::fd_in()
{
    cout << "fd in No.2" << endl;
    return (this->fd_in_);
}

FileDiscriptor WebservCgiEvent::fd_out()
{
    cout << "fd out No.1" << endl;
    return (this->fd_out_);
}

void WebservCgiEvent::set_pid(FileDiscriptor pid)
{
    this->pid_ = pid;
}

FileDiscriptor WebservCgiEvent::pid()
{
    cout << "pid in No.3" << endl;
    return (this->pid_);
}

void WebservCgiEvent::add_cgi_triger(CGI_Triger val)
{
    this->cgi_triger_ += val;
}

void WebservCgiEvent::clear_cgi_triger()
{
    this->cgi_triger_ = 0;
}

int WebservCgiEvent::cgi_triger()
{
    return (this->cgi_triger_);
}

bool WebservCgiEvent::is_cgi()
{
    return (this->is_cgi_);
}

void WebservCgiEvent::set_is_cgi(bool is_cgi)
{
    this->is_cgi_ = is_cgi;
}


bool WebservCgiEvent::is_completed()
{
    return (this->is_completed_);
}
