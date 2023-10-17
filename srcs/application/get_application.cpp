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

void GetApplication::execute()
{
    this->check_permission();
    std::string const &filename = this->get_requested_path();
    cout << "Get Application execute requetesd filename:" << filename << endl;
    cout << "Get Application execute cgi application:" << this->cgi_application_path << endl;
    //cout << filename << endl;
}

void GetApplication::check_permission()
{
    //cout << "limit size:" << this->location->get_limit_size() << endl;
    //cout << "root:" << this->location->root() << endl;
    //for(size_t i=0;i<this->location->get_limit_size(); i++){
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

GetApplication* GetApplication::from_location(const ConfigLocation *location, const Request *req, CGI *cgi)
{
    GetApplication *app = new GetApplication();
    app->location = location;
    app->req = req;
    app->cgi = cgi;

    try{
        app->cgi_application_path = string(cgi->get_cgi_application_path(req, location));
        app->is_cgi = true;
    }catch(std::invalid_argument &e){
        app->is_cgi = false;
    }

    return (app);
}

Response* GetApplication::make_response()
{
    return (new Response());
}
