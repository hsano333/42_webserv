#include "delete_application.hpp"
#include "http_exception.hpp"
#include "normal_reader.hpp"
#include "normal_file.hpp"
#include "directory_file.hpp"
#include "header_word.hpp"

DeleteApplication::DeleteApplication() : method(Method::from_string("DELETE"))
{
;
}

DeleteApplication::~DeleteApplication()
{
;
}

bool DeleteApplication::is_cgi() const
{
    return (false);
}


WebservEvent* DeleteApplication::next_event(WebservEvent *event, WebservEventFactory *event_factory)
{
    return (event_factory->make_making_response_event(event, event->entity()->io().destination(), NULL));
}

E_EpollEvent DeleteApplication::epoll_event(WebservEntity *entity)
{
    (void)entity;
    return (EPOLL_NONE);
}

bool DeleteApplication::execute(WebservEntity *entity)
{
    DEBUG("DeleteApplication::invoke");
    (void)entity;

    Request *req = entity->request();
    Config const *cfg = entity->config();
    ConfigServer const *server = cfg->get_server(req);
    ConfigLocation const *location = cfg->get_location(server, req);
    (void)location;

    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    FileDiscriptor const &fd = entity->fd();
    WebservFile *file = NULL;
    StatusCode code;

    if(req->is_file())
    {
        const string &file_path = req->requested_path();
        if(Utility::is_readable_file(file_path)){
            code = StatusCode::from_int(200);
            file = file_factory->make_normal_file(fd, file_path, std::ios::in | std::ios::binary);
            if(file->remove() < 0){
                MYINFO("Delete Error: can't remove");
                throw HttpException("403");
            }
        }else{
            MYINFO("Delete Error: not redable");
            throw HttpException("403");
        }
    }else if(req->is_directory()){
        MYINFO("Delete Error: not file, it's directory");
        throw HttpException("403");
    }else{
        MYINFO("Delete Error: neither file nor directory");
        throw HttpException("403");
    }

    ApplicationResult *result_ = ApplicationResult::from_status_code(entity->fd(), code, this->method);
    result_->add_header(CONTENT_LENGTH, "0");
    result_->set_file(NULL);
    entity->set_result(result_);
    return (true);
}

const Method &DeleteApplication::which() const
{
    return (this->method);
}

DeleteApplication *DeleteApplication::singleton = NULL;
DeleteApplication *DeleteApplication::get_instance()
{
    DEBUG("DeleteApplication::get_instance()");
    if (DeleteApplication::singleton == NULL){
        singleton = new DeleteApplication();
    }
    return (singleton);
}
