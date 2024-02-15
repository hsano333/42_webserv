#include "webserv_cleaner.hpp"
#include "webserv_clean_event.hpp"
#include "webserv_read_event.hpp"
#include "webserv_nothing_event.hpp"
#include "webserv_keep_alive_event.hpp"

using std::cout;
using std::endl;

WebservCleaner::WebservCleaner(
                    IOMultiplexing *io_multi_controller,
                    EventManager *event_manager,
                    FDManager *fd_manager
                    ):
                    io_multi_controller(io_multi_controller),
                    event_manager(event_manager),
                    fd_manager(fd_manager)
{
    ;
}
WebservCleaner::~WebservCleaner()
{
    ;
}

// Socket fd is not cleaned
void WebservCleaner::clean(WebservEvent *event, bool force_close)
{
    //bool force_close = event->entity()->force_close();
    //WebservCleanEvent *app_event = static_cast<WebservCleanEvent*>(event);
    //EventPointer app_event = event->event();
    WebservEntity *entity = event->entity();
    FileDiscriptor const &fd = entity->fd();
    DEBUG("WebservCleaner::clean:" + fd.to_string());

    bool is_close = force_close || entity->force_close();
    cout << "force_close=" << force_close << endl;
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
        this->fd_manager->close_fd(fd);
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
void force_clean(WebservTimeoutEvent *event, FileDiscriptor const &fd)
{
    (void)event;
    //bool force_close = event->entity()->force_close();
    //WebservCleanEvent *app_event = static_cast<WebservCleanEvent*>(event);
    //EventPointer app_event = event->event();
    //WebservEntity *entity = event->entity();
    //FileDiscriptor const &fd = entity->fd();
    DEBUG("WebservCleaner::clean:" + fd.to_string());

    //bool is_close =  entity->force_close();
    //cout << "force_close=" << force_close << endl;
    //cout << "app_event->is_force_close()=" << app_event->is_force_close() << endl;
    cout << "test No.1" << endl;
    //Request *req = entity->request();
    //Response *res = entity->response();
    //(void)res;


    //entity->clean();
    //event->close_fd(fd);
    event->close_fd(fd);


    MYINFO("close fd:" + fd.to_string());
    //ヘッダーでcloseするように指定されているので、closeする
    //this->io_multi_controller->erase(app_fd);
    event->clean();
    //this->fd_manager->close_fd(fd);
    //entity->set_completed(true);
}
*/


//class WebservEntity;
#include "webserv_timeout_event.hpp"
#include "webserv_entity.hpp"
void clean_timeout_events(WebservTimeoutEvent *event, WebservEntity *entity)
{
    (void)event;
    std::vector<WebservEvent *> timeout_events;

    event->event_manager->retrieve_timeout_events(timeout_events);
    for(size_t i=0;i<timeout_events.size();i++){
        //force_clean(timeout_events[i], entity->entity()->fd());
        event->close_fd(entity->fd());
    }
    entity->set_completed(true);
}

/*
void WebservCleaner::clean_timeout_events(WebservEvent *event)
{
    (void)event;
    std::vector<WebservEvent *> timeout_events;

    this->event_manager->retrieve_timeout_events(timeout_events);
    for(size_t i=0;i<timeout_events.size();i++){
        this->clean(timeout_events[i], true);
    }
    event->entity()->set_completed(true);
}
*/

