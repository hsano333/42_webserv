#include "get_application.hpp"
#include "http_exception.hpp"
#include "normal_reader.hpp"

GetApplication::GetApplication()
{
;
}

GetApplication::~GetApplication()
{
;
}

std::string const &GetApplication::get_requested_path() const
{
    if (this->is_cgi){
        return (this->req->requested_filepath());
    }

    if (this->req->is_file()){
        return (this->req->requested_filepath());
    }
    return (this->req->requested_path());
}


/*
bool GetApplication::is_cgi() const
{
    return (this->cgi->is_cgi(this->req, this->location));
}
*/

void GetApplication::execute_not_cgi()
{
    // Nothing to do
    return;
    if(this->server->is_redirect())
    {
        DEBUG("Redirect");
        // Nothing to do
        return;
        //this->res_status_code = this->server->first;
    }

    //cout << "filename:" << filename << endl;
}


void GetApplication::execute_cgi()
{

}

void GetApplication::execute()
{
    this->check_permission();
    if (this->is_cgi){
        this->execute_cgi();
    }else{
        this->execute_not_cgi();
    }

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

    try{
        app->cgi_application_path = string(cgi->get_cgi_application_path(req, app->location));
        app->is_cgi = true;
    }catch(std::invalid_argument &e){
        app->is_cgi = false;
    }

    return (app);
}

Response* GetApplication::make_response()
{
    std::string const &filepath = this->get_requested_path();

    if(this->server->is_redirect()){
        return (Response::from_file(filepath));
    }else if (this->req->is_file()){
        return (Response::from_file(filepath));
    }else if (this->req->is_directory()){
        return (Response::from_directory(filepath));
    }

    ERROR("GetApplication::make_response(): Neither file nor directory");
    throw HttpException("500");
}



