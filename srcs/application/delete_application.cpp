#include "delete_application.hpp"
#include "http_exception.hpp"
#include "normal_reader.hpp"
#include "normal_file.hpp"
#include "directory_file.hpp"

DeleteApplication::DeleteApplication()
{
;
}

DeleteApplication::~DeleteApplication()
{
;
}


File *DeleteApplication::get_requested_file()
{
    File *file = NULL;
    try{
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

bool DeleteApplication::execute()
{
    DEBUG("DeleteApplication::execute()");
    this->check_permission();
    File *file = this->get_requested_file();

    if(file->remove() < 0)
    {
        ERROR("failure to delete file:" + file->path());
        throw HttpException("403");
    }
    return (true);
}

void DeleteApplication::check_permission()
{
    const ConfigLimit *limit = this->location->limit();
    if (limit == NULL){
        WARNING("no permission: root:" + this->location->root());
        throw HttpException("403");
    }
    /*
    if (this->req->is_directory()){
        WARNING("can't delete directory:" + this->location->root());
        throw HttpException("403");
    }
    */

    std::vector<Method> methods = limit->allowed_method();
    Method requested_method = this->req->req_line().method();
    for(size_t j = 0;j<methods.size();j++){
        if (requested_method == methods[j]){
            return ;
        }
    }
    WARNING("not permission Delete Method: refer to config file :" + this->location->root());
    throw HttpException("403");
}

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


