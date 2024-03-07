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


WebservFile *DeleteApplication::get_requested_file(FileDiscriptor const &fd)
{
    WebservFile *file = NULL;
    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    try{
        if (this->req->is_not_executable_parent_dir()){
            ERROR("Parent directory is not x permission:" + this->req->parent_dir_path());
            throw HttpException("403");
        }else if(this->req->is_directory()){
            std::string const &host = this->req->header().get_host();
            std::string const &relative_path= this->req->req_line().uri().path();
            //file = DirectoryFile::from_path(this->req->requested_path(), relative_path, host);
            file =  file_factory->make_directory_file(fd, this->req->requested_path(), relative_path, host);
            return (file);
        }else if (Utility::is_regular_file(this->req->requested_path())){
            //file = NormalFile::from_filepath(this->req->requested_path(), std::ios::in | std::ios::binary);
            file =  file_factory->make_normal_file(fd, this->req->requested_path(), std::ios::in | std::ios::binary);
            return (file);
        }
        ERROR("File does not exist:" + this->req->requested_path());
        throw HttpException("404");
    }catch(std::invalid_argument &e){
        delete file;
        ERROR("DeleteApplication::get_requested_file:" + string(e.what()));
        throw HttpException("404");
    }
}


bool DeleteApplication::is_cgi() const
{
    return (false);
}

/*
void DeleteApplication::execute_not_cgi()
{
    //this->tmp_headers.insert(std::make_pair("Date", Utility::time_to_string()));
    //this->tmp_headers.insert(std::make_pair("Server", WEBSERV_VERSION));
    //this->tmp_headers.insert(std::make_pair("Content-Length", Utility::get_file_size());
    // Nothing to do
    return;
    if(this->location->is_redirect())
    {
        DEBUG("Redirect");
        // Nothing to do
        return;
        //this->res_status_code = this->server->first;
    }

    //cout << "filename:" << filename << endl;
}


void DeleteApplication::execute_cgi()
{

}
*/

bool DeleteApplication::invoke(WebservEntity *entity)
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
    DEBUG("DeleteApplication::invoke No.1");
    if(req->is_file())
    {
        const string &file_path = req->requested_path();
    DEBUG("DeleteApplication::invoke No.2");
        if(Utility::is_readable_file(file_path)){
    DEBUG("DeleteApplication::invoke No.3");
            code = StatusCode::from_int(200);
    DEBUG("DeleteApplication::invoke No.4");
            file = file_factory->make_normal_file(fd, file_path, std::ios::in);
    DEBUG("DeleteApplication::invoke No.5");
            if(file->remove() < 0){
    DEBUG("DeleteApplication::invoke No.6");
                MYINFO("Delete Error: can't remove");
                throw HttpException("403");
            }
    DEBUG("DeleteApplication::invoke No.7");
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

    DEBUG("DeleteApplication::invoke No.8");
    ApplicationResult *result_ = ApplicationResult::from_status_code(code);
    result_->add_header(CONTENT_LENGTH, "0");
    result_->set_file(NULL);
    DEBUG("DeleteApplication::invoke No.9");
    entity->set_result(result_);
    DEBUG("DeleteApplication::invoke No.10");
    return (true);
}

bool DeleteApplication::execute(WebservEvent *event)
{
    (void)event;
    DEBUG("DeleteApplication::execute()");
    WebservFile *file = this->get_requested_file(event->entity()->fd());
    
    //ApplicationResult *result = ApplicationResult();
    //ApplicationResult *result = ApplicationResult::from_result();
    //result->set_file(file);


    //todo 
    return (true);

    //result->set_completed(true);

    if(file->remove() < 0)
    {
        //ERROR("failure to delete file:" + file->path());
        throw HttpException("403");
    }
    //if(file){
        //delete file;
    //}
    //return (result);
}

/*
ApplicationResult *DeleteApplication::get_result()
{
    //ApplicationResult *file = ApplicationResult::from_result();
    //return (file);
    return (NULL);
}
*/


/*
DeleteApplication* DeleteApplication::from_location(const Config *cfg, const Request *req)
{
    DeleteApplication *app = new DeleteApplication();
    app->cfg = cfg;
    app->server = cfg->get_server(req);
    //const ConfigLocation *location= this->cfg->get_location(server, req);
    app->location = cfg->get_location(app->server, req);
    app->req = req;
    //app->cgi = cgi;
    //app->path_info_ = req->tmp_path_info();

    return (app);
}
*/

/*
Response* DeleteApplication::make_response()
{
    StatusCode code = StatusCode::from_int(200);
    Response *res = Response::from_success_status_code(code, NULL);

    std::map<std::string, std::string>::iterator ite = this->tmp_headers.begin();
    std::map<std::string, std::string>::iterator end = this->tmp_headers.end();
    while(ite != end){
        res->add_header(ite->first, ite->second);
        ite++;
    }
    return (res);
}
*/

const Method &DeleteApplication::which() const
{
    return (this->method);
}


/*
void DeleteApplication::set_path_info(std::string const &path_info)
{
    this->path_info_ = path_info;
}

std::string &DeleteApplication::get_path_info()
{
    return (this->path_info_);
}

std::string const &DeleteApplication::path_info() const
{
    return (this->path_info_);
}
*/


DeleteApplication *DeleteApplication::singleton = NULL;
DeleteApplication *DeleteApplication::get_instance()
{
    DEBUG("DeleteApplication::get_instance()");
    if (DeleteApplication::singleton == NULL){
        singleton = new DeleteApplication();
    }
    return (singleton);
}
