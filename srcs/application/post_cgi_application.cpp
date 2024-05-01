#include "post_cgi_application.hpp"
#include "http_exception.hpp"
#include "normal_reader.hpp"
#include "normal_file.hpp"
#include "directory_file.hpp"
#include "connection_exception.hpp"
#include "application_result.hpp"

PostCGIApplication::PostCGIApplication() : res(NULL), method(Method::from_string("POST"))
{
;
}

PostCGIApplication::~PostCGIApplication()
{
;
}

WebservFile *PostCGIApplication::get_requested_file(FileDiscriptor const &fd)
{
    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    return (NULL);
    /*
    if (this->event->file()){
        return (this->event->file());
    }
    */

    //File *file = NULL;
        //this->req->print_info();
        if (this->req->is_not_executable_parent_dir()){
            ERROR("Parent directory is not x permission:" + this->req->parent_dir_path());
            throw HttpException("403");
        }
        else if(this->req->is_directory()){
            ERROR("directory already exist :" + this->req->parent_dir_path());
            throw HttpException("403");
        }else if (Utility::is_regular_file(this->req->requested_path())){
            ERROR("file already exist :" + this->req->requested_path());
            throw HttpException("403");
        }
        //WebservFile *file = NormalFile::from_filepath(this->req->requested_path(), std::ios::out | std::ios::binary);
        WebservFile *file =  file_factory->make_normal_file(fd, this->req->requested_path(), std::ios::out | std::ios::binary);
        //this->event->set_file(file);
        return (file);
}


bool PostCGIApplication::is_cgi() const
{
    return (true);
}

bool PostCGIApplication::check_not_cgi_end(size_t received_size)
{
    (void)received_size;

    return (true);
}


WebservEvent* PostCGIApplication::next_event(WebservEvent *event, WebservEventFactory *event_factory)
{
    return (event_factory->make_making_upload_event(event, event->entity()->io().source()));
}

E_EpollEvent PostCGIApplication::epoll_event(WebservEntity *entity)
{
    (void)entity;
    return (EPOLL_NONE);
}

bool PostCGIApplication::execute(WebservEntity *entity)
{
    (void)entity;
    return (false);
}


ApplicationResult *PostCGIApplication::get_result()
{
    //ApplicationResult *file = ApplicationResult::from_result();
    //return (file);
    return (NULL);
}

PostCGIApplication* PostCGIApplication::from_location(const Config *cfg, WebservEvent *event, CGI *cgi)
{
    DEBUG("PostCGIApplication::from_location");

    PostCGIApplication *app = new PostCGIApplication();
    app->cfg = cfg;
    app->req = static_cast<Request const*>(event->entity()->request());
    app->server = cfg->get_server(app->req);
    app->location = cfg->get_location(app->server, app->req);
    app->cgi = cgi;
    app->path_info_ = app->location->root() + "/" + app->req->tmp_path_info();
    //app->res = NULL;
    //app->is_continued = is_continued;
    //if(event->res()){
        //app->file = event->res()->get_file();
    //}
    app->event = event;
    //app->reader = reader;


    return (app);
}

Response* PostCGIApplication::make_response()
{
    StatusCode code = StatusCode::from_int(200);
    if (this->res){
        return (this->res);
    }
    return (Response::from_success_status_code(code, NULL));
    //return (Response::from_success_status_code(code));

    /*
    DEBUG("PostCGIApplication::make_response()");
    File *file = this->get_requested_file();
    Response *res = NULL;

    if(this->location->is_redirect()){
        res = Response::from_file(file);
    }else if (this->req->is_file() || this->req->is_directory()){
        res = Response::from_file(file);
    }else if (this->req->is_directory()){
        res = Response::from_file(file);
    }else{
        ERROR("PostCGIApplication::make_response(): Neither file nor directory");
        delete file;
        delete res;
        throw HttpException("403");
    }
    res->set_exist_body(true);

    std::map<std::string, std::string>::iterator ite = this->tmp_headers.begin();
    std::map<std::string, std::string>::iterator end = this->tmp_headers.end();
    while(ite != end){
        res->add_header(ite->first, ite->second);
        ite++;
    }
    return (res);
    */
}

const Method &PostCGIApplication::which() const
{
    return (this->method);
}

PostCGIApplication *PostCGIApplication::singleton = NULL;
PostCGIApplication *PostCGIApplication::get_instance()
{
    if (PostCGIApplication::singleton == NULL){
        singleton = new PostCGIApplication();
    }
    return (singleton);
}


/*
void PostCGIApplication::set_path_info(std::string const &path_info)
{
    this->path_info_ = path_info;
}

std::string &PostCGIApplication::get_path_info()
{
    return (this->path_info_);
}

std::string const &PostCGIApplication::path_info() const
{
    return (this->path_info_);
}
*/


