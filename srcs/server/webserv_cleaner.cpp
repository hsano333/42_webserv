#include "webserv_cleaner.hpp"
#include "webserv_clean_event.hpp"
#include "webserv_nothing_event.hpp"
#include "header_word.hpp"
#include "webserv_timeout_event.hpp"
#include "webserv_entity.hpp"


using std::cout;
using std::endl;

WebservCleaner::WebservCleaner(
                    IOMultiplexing *io_multi_controller,
                    EventManager *event_manager,
                    FDManager *fd_manager,
                    FileManager *file_manager
                    ):
                    io_multi_controller(io_multi_controller),
                    event_manager(event_manager),
                    fd_manager(fd_manager),
                    file_manager(file_manager)
{
    ;
}
WebservCleaner::~WebservCleaner()
{
    ;
}

void WebservCleaner::clean_timeout_events()
{
    DEBUG("WebservCleaner::clean_timeout_events()");
    std::set<WebservEvent *> timeout_events;

    this->event_manager->retrieve_clean_events(timeout_events);

    std::set<WebservEvent *>::iterator ite = timeout_events.begin();
    std::set<WebservEvent *>::iterator end = timeout_events.end();

    //for(size_t i=0;i<timeout_events.size();i++){
    while(ite != end){
        this->clean((*ite)->entity(), true);
    }
}

void WebservCleaner::clean(WebservEntity *entity, bool force_close)
{
    DEBUG("WebservCleaner::clean()");

    //entityを削除すると、後工程で問題が発生するので、ここでは消さない
    bool is_close = force_close || entity->force_close();
    if (entity->request()){
        std::string const &conect = entity->request()->header().find(CONNECTION);
        if (conect == "close"){
            is_close = true;
        }
    }

    entity->set_force_close(is_close);
    entity->set_completed(true);
}

void WebservCleaner::delete_event(WebservEvent *event)
{
    (void)event;
    DEBUG("WebservCleaner::delete_event");
}

void WebservCleaner::close_fd(FileDiscriptor const &fd)
{
    DEBUG("WebservCleaner::close fd:" + fd.to_string());
    fd_manager->close_fd(fd);
}


