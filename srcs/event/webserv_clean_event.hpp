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
        E_EpollEvent get_next_epoll_event(WebservEvent *event);

        static WebservCleanEvent *from_fd(FileDiscriptor fd);
        static WebservEvent *from_webserv_event(WebservEvent *event, bool force_close, FDManager *fd_manager);
        void close_fd(FileDiscriptor const &fd);

    private:
        WebservCleanEvent();
        static WebservCleanEvent *singleton;
        static WebservCleanEvent *get_instance(FDManager* fd_manager);
        FDManager *fd_manager;
};

#endif
