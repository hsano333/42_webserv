#include "get_application.hpp"
#include "http_exception.hpp"
#include "normal_reader.hpp"
#include "normal_file.hpp"
#include "directory_file.hpp"

GetApplication::GetApplication()
{
;
}

GetApplication::~GetApplication()
{
;
}

File *GetApplication::get_requested_file()
{
    if (this->is_cgi_){
        //todo
        return (NULL);
    }
    File *file = NULL;
    try{
        //this->req->print_info();

        if (this->req->is_not_executable_parent_dir()){
            ERROR("Parent directory is not x permission:" + this->req->parent_dir_path());
            throw HttpException("403");
        }else if(this->req->is_directory()){
            std::string const &host = this->req->header().get_host();
            std::string const &relative_path= this->req->req_line().uri().path();
            File *file = DirectoryFile::from_path(this->req->requested_path(), relative_path, host);
            return (file);
        }else if (Utility::is_regular_file(this->req->requested_path())){
            File *file = NormalFile::from_filepath(this->req->requested_path(), std::ios::in | std::ios::binary);
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


bool GetApplication::is_cgi() const
{
    return (this->is_cgi_);
}

bool GetApplication::execute_not_cgi()
{
    //this->tmp_headers.insert(std::make_pair("Date", Utility::time_to_string()));
    //this->tmp_headers.insert(std::make_pair("Server", WEBSERV_VERSION));
    //this->tmp_headers.insert(std::make_pair("Content-Length", Utility::get_file_size());
    // Nothing to do
    return (true);
    if(this->location->is_redirect())
    {
        DEBUG("Redirect");
        // Nothing to do
        return (true);
        //this->res_status_code = this->server->first;
    }

    //cout << "filename:" << filename << endl;
}


bool GetApplication::execute_cgi()
{
    return (true);
}

bool GetApplication::execute()
{
    this->check_permission();
    if (this->is_cgi_){
        DEBUG("GetApplication::execute() CGI");
        return (this->execute_cgi());
    }else{
        DEBUG("GetApplication::execute() Not CGI");
        return (this->execute_not_cgi());
    }
    return (true);

    //cout << "Get Application execute requetesd filename:" << filename << endl;
    //cout << "Get Application execute cgi application:" << this->cgi_application_path << endl;
    //cout << filename << endl;
}

void GetApplication::check_permission()
{
    const ConfigLimit *limit = this->location->limit();
    if (limit == NULL){
        WARNING("not permission: root:" + this->location->root());
        throw HttpException("403");
    }
    std::vector<Method> methods = limit->allowed_method();
    Method requested_method = this->req->req_line().method();
    for(size_t j = 0;j<methods.size();j++){
        if (requested_method == methods[j]){
            return ;
        }
    }
    WARNING("not permission: root:" + this->location->root());
    throw HttpException("403");
}

GetApplication* GetApplication::from_location(const Config *cfg, const Request *req, CGI *cgi)
{
    GetApplication *app = new GetApplication();
    app->cfg = cfg;
    app->server = cfg->get_server(req);
    //const ConfigLocation *location= this->cfg->get_location(server, req);
    app->location = cfg->get_location(app->server, req);
    app->req = req;
    app->cgi = cgi;
    app->path_info_ = req->tmp_path_info();

    try{
        app->cgi_application_path = string(cgi->get_cgi_application_path(req, app->location));
        app->is_cgi_ = true;
    }catch(std::invalid_argument &e){
        app->is_cgi_ = false;
    }

    return (app);
}

Response* GetApplication::make_response()
{
    DEBUG("GetApplication::make_response()");
    File *file = this->get_requested_file();
    Response *res = NULL;

    if(this->location->is_redirect()){
        res = Response::from_file(file);
    }else if (this->req->is_file() || this->req->is_directory()){
        res = Response::from_file(file);
    }else if (this->req->is_directory()){
        res = Response::from_file(file);
    }else{
        ERROR("GetApplication::make_response(): Neither file nor directory");
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
}

/*
void GetApplication::set_path_info(std::string const &path_info)
{
    this->path_info_ = path_info;
}

std::string &GetApplication::get_path_info()
{
    return (this->path_info_);
}

std::string const &GetApplication::path_info() const
{
    return (this->path_info_);
}
*/


