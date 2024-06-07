#include "get_application.hpp"
#include "http_exception.hpp"
#include "normal_reader.hpp"
#include "normal_file.hpp"
#include "directory_file.hpp"
#include "header_extension.hpp"
#include "utility.hpp"
#include "header_word.hpp"
#include "error_file.hpp"
#include <unistd.h>

GetApplication::GetApplication() : method(Method::from_string("GET"))
{
    ;
}

GetApplication::~GetApplication()
{
;
}

bool GetApplication::is_cgi() const
{
    return (false);
}

string GetApplication::check_content(string const &filepath)
{
    string ext = Utility::get_extension(filepath);
    DEBUG("GetApplication::check_content ext:" + ext);
    if(ext == TEXT_PLAIN){
        return (CONTENT_TEXT_PLAIN);
    }else if(ext == TEXT_HTML){
        return (CONTENT_TEXT_HTML);
    }else if(ext == TEXT_CSS){
        return (CONTENT_TEXT_CSS);
    }else if(ext == TEXT_JAVASCRIPT){
        return (CONTENT_TEXT_JAVASCRIPT);
    }else if(ext == APP_JSON){
        return (CONTENT_APPLICATION_JSON);
    }else if(ext == APP_EXE){
        return (CONTENT_APPLICATION_EXE);
    }else if(ext == APP_PDF){
        return (CONTENT_APPLICATION_PDF);
    }else if(ext == APP_GZIP){
        return (CONTENT_APPLICATION_GZIP);
    }else if(ext == APP_ZIP){
        return (CONTENT_APPLICATION_ZIP);
    }else if(ext == APP_TAR){
        return (CONTENT_APPLICATION_TAR);
    }else if(ext == IMAGE_PNG){
        return (CONTENT_IMAEG_PNG);
    }else if(ext == IMAGE_GIF){
        return (CONTENT_IMAEG_GIF);
    }else if(ext == IMAGE_JPEG){
        return (CONTENT_IMAEG_JPEG);
    }else if(ext == IMAGE_BMP){
        return (CONTENT_IMAEG_BMP);
    }else if(ext == IMAGE_SVG){
        return (CONTENT_IMAGE_SVG);
    }else if(ext == AUDIO_MP3){
        return (CONTENT_AUDIO_MP3);
    }
    return ("");
}


string GetApplication::get_index_path(Request *req, ConfigLocation const *location, bool *is_existed)
{
    DEBUG("get_index_path has_index:" + location->index());
    if(location->index()){
        string index_path = req->requested_path() + "/" +  location->index_file();
        if(Utility::is_regular_file(index_path)){
            *is_existed = true;
            if(Utility::is_readable_file(index_path)){
                return (index_path);
            }
        }
    }
    return ("");
}

string GetApplication::get_default_index_path(Request *req, bool *is_existed)
{
    string index_path = req->requested_path() + "/index.html";
    DEBUG("default index_path=" + index_path);
    if(Utility::is_regular_file(index_path)){
        if(Utility::is_readable_file(index_path)){
            *is_existed = true;
            return (index_path);
        }
    }
    return ("");
}


WebservEvent* GetApplication::next_event(WebservEvent *event, WebservEventFactory *event_factory)
{
    return (event_factory->make_making_response_event(event, event->entity()->io().destination(), NULL));
}

E_EpollEvent GetApplication::epoll_event(WebservEntity *entity)
{
    (void)entity;
    return (EPOLL_NONE);
}

bool GetApplication::execute(WebservEntity *entity)
{
    DEBUG("GetApplication::invoke()");

    Request *req = entity->request();
    Config const *cfg = entity->config();
    ConfigServer const *server = cfg->get_server(req);
    ConfigLocation const *location = cfg->get_location(server, req);

    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    FileDiscriptor const &fd = entity->fd();
    WebservFile *file = NULL;
    string extension = "";
    StatusCode code;
    bool is_directory = false;

    if(req->is_file())
    {
        const string &file_path = req->requested_path();
        if(Utility::is_readable_file(file_path)){
            code = StatusCode::from_int(200);
            file = file_factory->make_normal_file(fd, file_path, std::ios::in | std::ios::binary);
            extension = GetApplication::check_content(file_path);
        }else{
            throw HttpException("403");
        }
    }else if(req->is_directory()){
        bool is_existed = false;
        string index_path = get_index_path(req, location, &is_existed);

        if(index_path != ""){
            file = file_factory->make_normal_file(fd, index_path, std::ios::in | std::ios::binary);
            code = StatusCode::from_int(200);
            extension = GetApplication::check_content(index_path);
        }else if(is_existed){
            throw HttpException("403");
        }else if(location->autoindex()){
            std::string const &host = req->header().get_host();
            std::string const &relative_path= req->req_line().uri().path();
            WebservFile *directory_file = file_factory->make_directory_file(fd, req->requested_path(), relative_path, host);
            file = file_factory->make_socket_chunk_file_for_autoindex(entity->fd(), directory_file);
            file->set_chunk(true);
            code = StatusCode::from_int(200);
            is_directory = true;
        }
        else
        {
            is_existed = false;
            string index_path = get_default_index_path(req, &is_existed);

            if(index_path != ""){
                file = file_factory->make_normal_file(fd, index_path, std::ios::in | std::ios::binary);
                code = StatusCode::from_int(200);
                extension = GetApplication::check_content(index_path);
            }else if(is_existed){
                ERROR("403 Error");
                throw HttpException("403");
            }else{
                ERROR("404 Error");
                throw HttpException("404");
            }
        }
    }else{
        ERROR("404 Error");
        throw HttpException("404");
    }

    ApplicationResult *result_ = ApplicationResult::from_status_code(entity->fd(), code, this->method);
    if(extension != ""){
        result_->add_header(CONTENT_TYPE, extension);
    }
    if(file){
        if(is_directory){
            result_->add_header(TRANSFER_ENCODING, TRANSFER_ENCODING_CHUNKED);
        }else{
            string file_size = Utility::to_string(Utility::get_file_size(file->path()));
            result_->add_header(CONTENT_LENGTH, file_size);
        }
    }
    result_->set_file(file);
    entity->set_result(result_);
    return (true);
}

const Method &GetApplication::which() const
{
    return (this->method);
}

GetApplication *GetApplication::singleton = NULL;
GetApplication *GetApplication::get_instance()
{
    if (GetApplication::singleton == NULL){
        singleton = new GetApplication();
    }
    return (singleton);
}
