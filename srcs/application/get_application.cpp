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

/*
WebservFile *GetApplication::get_requested_file(FileDiscriptor const &fd)
{
    DEBUG("GetApplication::get_requested_file()");
    WebservFile *file = NULL;
    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    try{
        if (this->req->is_not_executable_parent_dir()){
            ERROR("Parent directory is not x permission:" + this->req->parent_dir_path());
            throw HttpException("403");
        }else if(this->req->is_directory()){
            std::string const &host = this->req->header().get_host();
            std::string const &relative_path= this->req->req_line().uri().path();
            file = file_factory->make_directory_file(fd, this->req->requested_path(), relative_path, host);  DirectoryFile::from_path(this->req->requested_path(), relative_path, host);
            if(this->location->autoindex()){
                return (file);
            }
            delete file;
            std::vector<std::string> const &index_pathes = this->location->indexes();

            if (index_pathes.size() == 0){
                ERROR("autoindex is OFF and index file is not set");
                throw HttpException("403");
            }
            std::string index_path = this->location->root() + "/" + index_pathes[0];
            MYINFO("index_path:" + index_path);
            file =  file_factory->make_normal_file(fd, index_path, std::ios::in | std::ios::binary);
            return (file);

        }else if (Utility::is_regular_file(this->req->requested_path())){
            file =  file_factory->make_normal_file(fd, this->req->requested_path(), std::ios::in | std::ios::binary);
            return (file);
        }
        ERROR("File does not exist:" + this->req->requested_path());
        throw HttpException("404");
    }catch(std::invalid_argument &e){
        delete file;
        ERROR("GetApplication::get_requested_file:" + string(e.what()));
        throw HttpException("404");
    }
}
*/


bool GetApplication::is_cgi() const
{
    return (false);
}

/*
ApplicationResult *GetApplication::get_result()
{
    return (this->result_);
}
*/


string GetApplication::check_content(string const &filepath)
{
    string ext = Utility::get_extension(filepath);
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
    }else if(ext == AUDIO_MP3){
        return (CONTENT_AUDIO_MP3);
    }
    return ("");
}


string GetApplication::get_index_path(Request *req, ConfigLocation const *location, bool *is_existed)
{
    for(size_t i=0;i<location->indexes().size();i++){
        string index_path = req->requested_path() + location->indexes()[i];
        if(Utility::is_regular_file(index_path)){
            if(Utility::is_readable_file(index_path)){
                *is_existed = true;
                return (index_path);
            }
        }
    }
    return ("");

}


WebservEvent* GetApplication::next_event(WebservEvent *event, WebservEventFactory *event_factory)
{
    return (event_factory->make_making_response_event(event, event->entity()->io().destination()));
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
    //Request *req = entity->request();
    WebservFile *file = NULL;
    string extension = "";
    StatusCode code;
    bool is_directory = false;

    if(req->is_file())
    {
        const string &file_path = req->requested_path();
        if(Utility::is_readable_file(file_path)){
            code = StatusCode::from_int(200);
            file = file_factory->make_normal_file(fd, file_path, std::ios::in);
            extension = GetApplication::check_content(file_path);
        }else{
            throw HttpException("403");
        }
    }else if(req->is_directory()){
        bool is_existed = false;
        string index_path = get_index_path(req, location, &is_existed);

        if(index_path != ""){
            file = file_factory->make_normal_file(fd, index_path, std::ios::in);
            code = StatusCode::from_int(200);
            extension = GetApplication::check_content(index_path);
        }
        else
        {
            if(location->autoindex())
            {
                std::string const &host = req->header().get_host();
                std::string const &relative_path= req->req_line().uri().path();
                //file = DirectoryFile::from_path(req->requested_path(), relative_path, host);
                WebservFile *directory_file = file_factory->make_directory_file(fd, req->requested_path(), relative_path, host);
                file = file_factory->make_socket_chunk_file_for_write(entity->fd(), directory_file);
                file->set_chunk(true);
                code = StatusCode::from_int(200);
                is_directory = true;
            }
            else
            {
                if(is_existed){
                    throw HttpException("403");
                }else{
                    throw HttpException("404");
                }
            }
        }
    }else{
        throw HttpException("404");
        //code = StatusCode::from_int(404);
        //file = file_factory->make_error_file(fd, code);
    }

    ApplicationResult *result_ = ApplicationResult::from_status_code(code, this->method);
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


/*
bool GetApplication::execute(WebservEvent *event)
{
    DEBUG("GetApplication::execute()");

    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    FileDiscriptor const &fd = event->entity()->fd();
    Request *req = event->entity()->request();
    WebservFile *file = NULL;
    string extension = "";
    StatusCode code;
    bool is_directory = false;

    if(req->is_file())
    {
        const string &file_path = req->requested_path();
        if(Utility::is_readable_file(file_path)){
            code = StatusCode::from_int(200);
            //file = NormalFile::from_filepath(file_path, std::ios::in);
            file =  file_factory->make_normal_file(fd, file_path, std::ios::in);
            extension = GetApplication::check_content(file_path);
        }else{
            code = StatusCode::from_int(403);
        }
    }else if(req->is_directory()){
        bool is_existed = false;
        string index_path = get_index_path(req, &is_existed);

        if(index_path != ""){
            //file = NormalFile::from_filepath(index_path, std::ios::in);
            file =  file_factory->make_normal_file(fd, index_path, std::ios::in);
            code = StatusCode::from_int(200);
            extension = GetApplication::check_content(index_path);
        }
        else
        {
            if(this->location->autoindex())
            {
                std::string const &host = req->header().get_host();
                std::string const &relative_path= req->req_line().uri().path();
                //file = DirectoryFile::from_path(req->requested_path(), relative_path, host);
                file =  file_factory->make_directory_file(fd, req->requested_path(), relative_path, host);
                code = StatusCode::from_int(200);
                is_directory = true;
            }
            else
            {
                if(is_existed){
                    code = StatusCode::from_int(403);
                }else{
                    code = StatusCode::from_int(404);
                }
            }
        }
    }else{
        code = StatusCode::from_int(404);
        //file = ErrorFile::from_status_code(code);
        file = file_factory->make_error_file(fd, code);
    }

    this->result_ = ApplicationResult::from_status_code(code);
    if(extension != ""){
        this->result_->add_header(CONTENT_TYPE, extension);
    }
    if(file){
        if(is_directory){
            this->result_->add_header(TRANSFER_ENCODING, TRANSFER_ENCODING_CHUNKED);
        }else{
            string file_size = Utility::to_string(Utility::get_file_size(file->path()));
            this->result_->add_header(CONTENT_LENGTH, file_size);
        }
    }
    this->result_->set_file(file);
    return (true);
}
*/

/*
GetApplication* GetApplication::from_location(const Config *cfg, const Request *req)
{
    GetApplication *app = new GetApplication();
    app->cfg = cfg;
    app->server = cfg->get_server(req);
    app->location = cfg->get_location(app->server, req);
    app->req = req;

    return (app);
}
*/

/*
Response* GetApplication::make_response(FileDiscriptor const &fd)
{
    DEBUG("GetApplication::make_response()");
    WebservFile *file = this->get_requested_file(fd);
    if(file == NULL){
        cout << "file is NULL" << endl;
    }
    Response *res = NULL;

    if(this->location->is_redirect()){
        res = Response::from_file(file);
    }else if (this->req->is_file() || this->req->is_directory()){
        res = Response::from_file(file);
    }else if (this->req->is_directory()){
        res = Response::from_file(file);
    }else{
        ERROR("GetApplication::make_response(): Neither file nor directory");
        //delete file;
        delete res;
        throw HttpException("403");
    }

    std::map<std::string, std::string>::iterator ite = this->tmp_headers.begin();
    std::map<std::string, std::string>::iterator end = this->tmp_headers.end();
    while(ite != end){
        res->add_header(ite->first, ite->second);
        ite++;
    }
    return (res);
}
*/


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
