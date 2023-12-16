#include "get_application.hpp"
#include "http_exception.hpp"
#include "normal_reader.hpp"
#include "normal_file.hpp"
#include "directory_file.hpp"
#include <unistd.h>

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
    /*
    if (this->is_cgi_){
        File *file = CGIFile::from_fd(this->cgi_event_.cgi_fd(), READ_ONLY);
        return (file);
    }
    */
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
            file = NormalFile::from_filepath(index_path, std::ios::in | std::ios::binary);
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
    return (false);
}


//extern char** environ;

//bool GetApplication::execute_cgi()
//{
//
//    this->cgi_event_.set_is_cgi(true);
//    DEBUG("GetApplication::execute_cgi()");
//    //string &execve_path = this->cgi_application_path;
//    string const &exe_root = this->location->root();
//    string const &file_path = this->req->requested_filepath();
//    string const &query = this->req->req_line().uri().query();
//    string env_query = "QUERY_STRING:" + query;
//    string env_path_info = "PATH_INFO:" + this->path_info_;
//
//    char* argv[2] = {NULL};
//    //argv[0] = const_cast<char*>(execve_path.c_str());
//    argv[0] = const_cast<char*>(file_path.c_str());
//
//
//    char* env[3] = {NULL};
//    env[0] = const_cast<char*>(env_query.c_str());
//    env[1] = const_cast<char*>(env_path_info.c_str());
//
//
//    //char *env[];
//    printf("argv[0]=%s\n", argv[0]);
//    printf("argv[1]=%s\n", argv[1]);
//    printf("env[0]=%s\n", env[0]);
//    printf("env[1]=%s\n", env[1]);
//    printf("exe_root=%s\n", exe_root.c_str());
//
//    int fd_in;
//    int fd_out;
//    int pid = this->cgi->make_thread(&fd_in, &fd_out);
//    if(pid < 0){
//        ERROR("failre to create thread:");
//        throw HttpException("500");
//    }
//
//    if (pid == 0) {
//        int rval = execve(file_path.c_str(), argv, env);
//        cout << "rval:" << rval << endl;
//        std::exit(0);
//    }
//
//    printf("cgi pid parent\n");
//    printf("cgi pid parent\n");
//    printf("cgi pid parent\n");
//    printf("cgi pid parent\n");
//
//    // Get Method don't have body data, so nothing writing data to std-in in cgi
//    ::close(fd_in);
//    //this->cgi_out = FileDiscriptor::from_int(fd_out);
//    this->cgi_event_.set_pid(pid);
//    this->cgi_event_.set_cgi_fd(FileDiscriptor::from_int(fd_out));
//
//    /*
//    sleep(1);
//    char buf[1000]={0};
//    int rval = read(fd_out, buf, 1000);
//    if(rval < 0){
//        cout << "read error" << endl;
//        exit(0);
//    }
//    buf[rval] = '\0';
//    cout << "cgi buf=" << buf << endl;
//        exit(0);
//        */
//
//    //int rval = this->cgi->execute(fd_in, fd_out, pid);
//    //cout << "execve_path:" << execve_path << endl;
//    cout << "file_path:" << file_path << endl;
//    cout << "query:" << query << endl;
//    //exit(0);
//
//    //cgi->();
//    return (true);
//}

bool GetApplication::execute()
{
    this->check_permission();
    /*
    if (this->is_cgi_){
        DEBUG("GetApplication::execute() CGI");
        return (this->execute_cgi());
    }else{
        DEBUG("GetApplication::execute() Not CGI");
        return (this->execute_not_cgi());
    }
    */
    return (true);
    if(this->location->is_redirect())
    {
        DEBUG("Redirect");
        // Nothing to do
        return (true);
        //this->res_status_code = this->server->first;
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


GetApplication* GetApplication::from_location(const Config *cfg, const Request *req)
{
    GetApplication *app = new GetApplication();
    app->cfg = cfg;
    app->server = cfg->get_server(req);
    //const ConfigLocation *location= this->cfg->get_location(server, req);
    app->location = cfg->get_location(app->server, req);
    app->req = req;
    //app->cgi = cgi;
    //app->path_info_ = app->location->root() + "/" + app->req->tmp_path_info();

    /*
    try{
        cgi->check_cgi_application_path(req, app->location);
        app->is_cgi_ = true;
    }catch(std::invalid_argument &e){
        app->is_cgi_ = false;
    }
    */

    return (app);
}

Response* GetApplication::make_response()
{
    DEBUG("GetApplication::make_response()");
    File *file = this->get_requested_file();
    if(file == NULL){
        cout << "file is NULL" << endl;
    }
    //WebservCgiEvent cgi_event_();
    //if(this->is_cgi_){
        //this->
        //cgi_event_.set_cgi_fd();
    //}
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

//Response* GetApplication::make_response()
//Response* GetApplication::make_response()
WebservCgiEvent *GetApplication::cgi_event()
{
    return (NULL);
    //return (this->cgi_event_);

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


