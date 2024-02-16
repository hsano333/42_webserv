
#include "webserv_clean_event.hpp"
#include "webserv_nothing_event.hpp"
#include "global.hpp"
#include "webserv_cleaner.hpp"

WebservCleanEvent::WebservCleanEvent()
{
    ;
}

WebservCleanEvent::~WebservCleanEvent()
{
    ;
}

WebservEvent* WebservCleanEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    MYINFO("WebservCleanEvent::make_next_event");
    if (event->entity()->force_close()){
        MYINFO("WebservCleanEvent::make_next_event() >> NULL");
        return (NULL);
    }else{
        return (event_factory->make_keep_alive_event(event ));
    }
    (void)event_factory;
    (void)event;
    return (NULL);
}

E_EpollEvent WebservCleanEvent::get_next_epoll_event(WebservEvent *event)
{
    if (event->entity()->force_close()){
        return (EPOLL_CLOSE);
    }else{
        return (EPOLL_READ);
    }
}

WebservCleanEvent *WebservCleanEvent::singleton = NULL;
WebservCleanEvent *WebservCleanEvent::get_instance(WebservCleaner *cleaner)
{
    if (WebservCleanEvent::singleton == NULL){
        singleton = new WebservCleanEvent();
        singleton->cleaner = cleaner;
    }
    return (singleton);
}

void clean(WebservCleanEvent *event, WebservEntity *entity)
{
    (void)event;
    //bool force_close = event->entity()->force_close();
    //WebservCleanEvent *app_event = static_cast<WebservCleanEvent*>(event);
    //EventPointer app_event = event->event();
    //WebservEntity *entity = event->entity();
    FileDiscriptor const &fd = entity->fd();
    DEBUG("WebservCleaner::clean:" + fd.to_string());

    bool is_close =  entity->force_close();
    //cout << "force_close=" << force_close << endl;
    //cout << "app_event->is_force_close()=" << app_event->is_force_close() << endl;
    cout << "test No.1" << endl;
    Request *req = entity->request();
    Response *res = entity->response();
    (void)res;
    if (req){
        std::string const &conect = req->header().find("Connection");
        if (conect == "close"){
            is_close = true;
        }
    }
    entity->set_force_close(is_close);

    /*
    if(app_event->src() != req && app_event->src() != res){
        cout << "delete src" << endl;
        delete app_event->src();
    }
    if(app_event->dst() != req && app_event->dst() != res){
        cout << "delete dst" << endl;
        delete app_event->dst();
    }
    */

        entity->clean();
        cout << "delete req" << endl;
    //delete app_event->req();
        cout << "delete res" << endl;
    //delete app_event->res();
        cout << "delete end" << endl;


    //this->io_multi_controller->erase(app_fd);
    //
    //this->event_manager->erase_event_waiting_epoll(app_fd);
    //this->event_manager->erase_event_waiting_epoll(app_fd);
    if (is_close)
    {
        MYINFO("close fd:" + fd.to_string());
        //ヘッダーでcloseするように指定されているので、closeする
        //this->io_multi_controller->erase(app_fd);
        event->close_fd(fd);
        //this->fd_manager->close_fd(fd);
    }else{
        MYINFO("not close fd:" + fd.to_string());
        // HTTP1.1はデフォルトでコネクションを切断しない
        //
        //this->io_multi_controller->modify(app_fd, EPOLLIN);
        //WebservEvent *new_event = WebservKeepAliveEvent::from_fd(app_fd);
        //this->event_manager->add_event_waiting_epoll(app_fd, new_event);
        //
    }
    entity->set_completed(true);
}



/*
WebservEvent *WebservCleanEvent::from_webserv_event(WebservEvent *event, bool force_close, FDManager *fd_manager)
{
    DEBUG("WebservCleanEvent::from_webserv_event");
    WebservCleanEvent *clean_event = WebservCleanEvent::get_instance(fd_manager);
    WebservEvent *new_event =  new WebservEvent(clean_event, clean, event->entity());
    new_event->entity()->set_force_close(force_close);
    new_event->entity()->io().set_source(event->entity()->request());
    return (new_event);
}
*/

WebservEvent *WebservCleanEvent::from_event(WebservEvent *event, WebservCleaner *cleaner, bool force_close)
{
    DEBUG("WebservCleanEvent::from_webserv_event");
    WebservCleanEvent *clean_event = WebservCleanEvent::get_instance(cleaner);
    WebservEvent *new_event =  new WebservEvent(clean_event, clean, event->entity());
    new_event->entity()->set_force_close(force_close);
    new_event->entity()->io().set_source(event->entity()->request());
    return (new_event);
}


void WebservCleanEvent::close_fd(FileDiscriptor const &fd)
{
    this->cleaner->close_fd(fd);
}
