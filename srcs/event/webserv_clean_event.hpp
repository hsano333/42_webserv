#ifndef WEBSERV_CLEAN_EVENT
#define WEBSERV_CLEAN_EVENT
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "request.hpp"
#include "response.hpp"
#include "webserv_entity.hpp"
#include "fd_manager.hpp"
#include "webserv_cleaner.hpp"

class WebservCleanEvent
{
    public:
        ~WebservCleanEvent();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event(WebservEvent *event);

        static WebservCleanEvent *from_fd(FileDiscriptor fd);
        static WebservEvent *from_event(WebservEvent *event, WebservCleaner *cleaner, bool force_close);
        bool force_close();
        WebservCleaner *cleaner() const;

    private:
        WebservCleanEvent();
        static WebservCleanEvent *singleton;
        static WebservCleanEvent *get_instance(WebservCleaner *cleaner);
        bool force_close_;
        WebservCleaner *cleaner_;
        //FDManager *fd_manager;
};

#endif
