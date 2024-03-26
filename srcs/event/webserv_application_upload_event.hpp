#ifndef WEBSERV_APPLICATION_UPLOAD_EVENT
#define WEBSERV_APPLICATION_UPLOAD_EVENT
#include "webserv_event.hpp"
#include "webserv_entity.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"

class WebservApplicationUploadEvent
{
    public:
        ~WebservApplicationUploadEvent();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent epoll_event(WebservEvent *event);
        void        check_completed(WebservEntity * entity);
        static WebservEvent *from_event(WebservEvent *event, WebservFile *src, WebservFile *dst);

    private:
        WebservApplicationUploadEvent();
        static WebservApplicationUploadEvent *singleton;
        static WebservApplicationUploadEvent *get_instance();
};

#endif
