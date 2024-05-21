#ifndef WEBSERV_CLEAN_EVENT
#define WEBSERV_CLEAN_EVENT
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "request.hpp"
#include "response.hpp"
#include "webserv_entity.hpp"
#include "fd_manager.hpp"

class WebservCleanEvent
{
    public:
        ~WebservCleanEvent();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent epoll_event(WebservEvent *event);
        void        check_completed(WebservEntity * entity);

        static WebservCleanEvent *from_fd(FileDiscriptor fd);
        static WebservEvent *from_event(WebservEvent *event, bool force_close);

    private:
        WebservCleanEvent();
        static WebservCleanEvent *singleton;
        static WebservCleanEvent *get_instance();
};

#endif
