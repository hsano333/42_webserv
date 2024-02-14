#ifndef WEBSERV_CLEAN_EVENT
#define WEBSERV_CLEAN_EVENT
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "request.hpp"
#include "response.hpp"
#include "webserv_cgi_event.hpp"
#include "webserv_entity.hpp"

class WebservCleanEvent
{
    public:
        ~WebservCleanEvent();
        EWebservEvent which();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event(WebservEvent *event);

        static WebservCleanEvent *from_fd(FileDiscriptor fd);
        void increase_timeout_count(int count);
        int  timeout_count();
        static WebservEvent *from_webserv_event(WebservEvent *event, bool force_close);
        //FileDiscriptor  &fd();
        //Request         *req();
        //Response        *res();
        File            *src();
        File            *dst();
        void            switching_io(uint32_t epoll_event);
        void            set_src(File *file);
        void            set_dst(File *file);
        //void set_null_res_and_req();

        bool is_completed();
        void set_completed(bool flag);
        bool is_force_close();
        void set_force_close(bool flag);
        //void clean_res_and_req();

        void set_cgi_event(WebservCgiEvent *cgi_event);
        WebservCgiEvent *cgi_event();
        WebservEntity *entity();

    private:
        WebservCleanEvent();
        static WebservCleanEvent *singleton;
        static WebservCleanEvent *get_instance();
        //FileDiscriptor  fd_;
        //Request         *req_;
        //Response        *res_;
        //File            *source_file;
        //File            *destination_file;
        //int             timeout_count_;
        //bool            force_close;
        //bool is_completed_;

        //WebservCgiEvent *cgi_event_;
        //WebservEntity         *entity_;
};

#endif
