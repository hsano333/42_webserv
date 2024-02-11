#ifndef WEBSERV_CGI_EVENT_HPP
#define WEBSERV_CGI_EVENT_HPP

#include "file_discriptor.hpp"
#include "webserv_entity.hpp"

typedef enum CGI_Triger
{
    CGI_WAIT = 1,
    CGI_READ = 2,
    CGI_WRITE = 4,
}CGI_Triger;

class WebservCgiEvent
{
    public:

        WebservCgiEvent();
        ~WebservCgiEvent();
        WebservCgiEvent(WebservCgiEvent const &cgi_event);
        WebservCgiEvent& operator=(WebservCgiEvent const &cgi_event);

        //void set_cgi_fd(FileDiscriptor fd);
        //FileDiscriptor cgi_fd();
        void set_fd_in(FileDiscriptor fd);
        FileDiscriptor fd_in();
        void set_fd_out(FileDiscriptor fd);
        FileDiscriptor fd_out();

        void set_pid(FileDiscriptor pid);
        FileDiscriptor pid();

        void add_cgi_triger(CGI_Triger val);
        void clear_cgi_triger();
        int cgi_triger();
        bool is_cgi();
        void set_is_cgi(bool is_cgi);
        bool is_completed();
        //struct Entity;
        //WebservEntity *entity();

    private:
        FileDiscriptor  fd_in_;
        FileDiscriptor  fd_out_;
        FileDiscriptor  pid_;
        int             cgi_triger_;
        bool            is_cgi_;
        bool            is_completed_;

};

#endif
