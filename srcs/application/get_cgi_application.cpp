#include "get_cgi_application.hpp"
#include "http_exception.hpp"
#include "normal_reader.hpp"
#include "normal_file.hpp"
#include "cgi_file.hpp"
#include "directory_file.hpp"
#include "webserv_event.hpp"
#include <unistd.h>

GetCGIApplication::GetCGIApplication()
{
    ;
}

GetCGIApplication::~GetCGIApplication()
{
;
}

File *GetCGIApplication::get_requested_file()
{
    File *file = CGIFile::from_fd(this->cgi_event_->fd_in(), READ_ONLY);
    return (file);
}


bool GetCGIApplication::is_cgi() const
{
    return (true);
}


//extern char** environ;

bool GetCGIApplication::execute()
{

    //this->cgi_event_.set_is_cgi(true);
    DEBUG("GetCGIApplication::execute()");
    //string &execve_path = this->cgi_application_path;
    string const &exe_root = this->location->root();
    string const &file_path = this->req->requested_filepath();
    string const &query = this->req->req_line().uri().query();
    string env_query = "QUERY_STRING:" + query;
    string env_path_info = "PATH_INFO:" + this->path_info_;

    char* argv[2] = {NULL};
    //argv[0] = const_cast<char*>(execve_path.c_str());
    argv[0] = const_cast<char*>(file_path.c_str());


    char* env[3] = {NULL};
    env[0] = const_cast<char*>(env_query.c_str());
    env[1] = const_cast<char*>(env_path_info.c_str());

    //char *env[];
    printf("argv[0]=%s\n", argv[0]);
    printf("argv[1]=%s\n", argv[1]);
    printf("env[0]=%s\n", env[0]);
    printf("env[1]=%s\n", env[1]);
    printf("exe_root=%s\n", exe_root.c_str());

    int fd_in;
    int fd_out;
    int pid = this->cgi->make_thread(&fd_in, &fd_out);
    if(pid < 0){
        ERROR("failre to create thread:");
        throw HttpException("500");
    }

    if (pid == 0) {
        int rval = execve(file_path.c_str(), argv, env);
        cout << "rval:" << rval << endl;
        std::exit(0);
    }

    printf("cgi pid parent\n");
    printf("cgi pid parent\n");
    printf("cgi pid parent\n");
    printf("cgi pid parent\n");

    // Get Method don't have body data, so nothing writing data to std-in in cgi
    //::close(fd_in);


    //this->cgi_out = FileDiscriptor::from_int(fd_out);
    //
    /*
    this->cgi_event_.set_pid(pid);
    this->cgi_event_.set_cgi_fd(FileDiscriptor::from_int(fd_out));
    */
    //int fd_in;
    //int fd_out;
    this->cgi_event_ = new WebservCgiEvent();
    cout << "this->cgi_event_ No.1 = " << this->cgi_event_ << endl;
    //FileDiscriptor pid_fd = FileDiscriptor::from_int(pid);
    FileDiscriptor cgi_pid = FileDiscriptor::from_int(pid);
    FileDiscriptor cgi_fd_in = FileDiscriptor::from_int(fd_in);
    FileDiscriptor cgi_fd_out = FileDiscriptor::from_int(fd_out);
    this->cgi_event_->set_pid(cgi_pid);
    this->cgi_event_->set_fd_in(cgi_fd_in);
    this->cgi_event_->set_fd_out(cgi_fd_out);

    cout << "this->cgi_event_ No.2 = " << this->cgi_event_ << endl;
    //this->fd_manager->add_socket_and_epoll_fd(pid_fd, fdout_fd);
    //
    
    
    /*
    WebservEvent* cgi_events[2];
    cgi_events = this->event_factory->make_cgi_event(pid_fd, cgi_fd_in, cgi_fd_out, &(event[0]));
    this->event_manager->push(*events[0]);
    this->event_manager->push(*events[1]);
    */

    //WebservEvent[0]


    /*
    sleep(1);
    char buf[1000]={0};
    int rval = read(fd_out, buf, 1000);
    if(rval < 0){
        cout << "read error" << endl;
        exit(0);
    }
    buf[rval] = '\0';
    cout << "cgi buf=" << buf << endl;
        exit(0);
        */

    //int rval = this->cgi->execute(fd_in, fd_out, pid);
    //cout << "execve_path:" << execve_path << endl;
    cout << "file_path:" << file_path << endl;
    cout << "query:" << query << endl;
    cout << "this->cgi_event_ No.3 = " << this->cgi_event_ << endl;

    return (true);
}

/*
bool GetCGIApplication::execute()
{
    DEBUG("GetCGIApplication::execute()");
    cout << "test No.1" << endl;
    this->check_permission();
    cout << "test No.2" << endl;

    //this->cgi_event_->set_is_cgi(true);
    cout << "test No.3" << endl;
    DEBUG("GetCGIApplication::execute_cgi()");
    //string &execve_path = this->cgi_application_path;
    string const &exe_root = this->location->root();
    cout << "test No.4" << endl;
    string const &file_path = this->req->requested_filepath();
    cout << "test No.5" << endl;
    string const &query = this->req->req_line().uri().query();
    cout << "test No.6" << endl;
    string env_query = "QUERY_STRING:" + query;
    string env_path_info = "PATH_INFO:" + this->path_info_;

    char* argv[2] = {NULL};
    //argv[0] = const_cast<char*>(execve_path.c_str());
    argv[0] = const_cast<char*>(file_path.c_str());


    char* env[3] = {NULL};
    env[0] = const_cast<char*>(env_query.c_str());
    env[1] = const_cast<char*>(env_path_info.c_str());


    //char *env[];
    printf("argv[0]=%s\n", argv[0]);
    printf("argv[1]=%s\n", argv[1]);
    printf("env[0]=%s\n", env[0]);
    printf("env[1]=%s\n", env[1]);
    printf("exe_root=%s\n", exe_root.c_str());

    int fd_in;
    int fd_out;
    int pid = this->cgi->make_thread(&fd_in, &fd_out);
    if(pid < 0){
        ERROR("failre to create thread:");
        throw HttpException("500");
    }

    if (pid == 0) {
        int rval = execve(file_path.c_str(), argv, env);
        cout << "rval:" << rval << endl;
        std::exit(0);
    }

    printf("cgi pid parent\n");
    printf("cgi pid parent\n");
    printf("cgi pid parent\n");
    printf("cgi pid parent\n");

    // Get Method don't have body data, so nothing writing data to std-in in cgi
    ::close(fd_in);
    //this->cgi_out = FileDiscriptor::from_int(fd_out);
    this->cgi_event_->set_pid(pid);
    this->cgi_event_->set_cgi_fd(FileDiscriptor::from_int(fd_out));


    //int rval = this->cgi->execute(fd_in, fd_out, pid);
    //cout << "execve_path:" << execve_path << endl;
    cout << "file_path:" << file_path << endl;
    cout << "query:" << query << endl;
    //exit(0);

    //cgi->();
    return (true);
    return (true);

    //cout << "Get Application execute requetesd filename:" << filename << endl;
    //cout << "Get Application execute cgi application:" << this->cgi_application_path << endl;
    //cout << filename << endl;
}
*/

void GetCGIApplication::check_permission()
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

GetCGIApplication* GetCGIApplication::from_location(const Config *cfg, const Request *req, CGI *cgi)
{
    GetCGIApplication *app = new GetCGIApplication();
    app->cfg = cfg;
    app->server = cfg->get_server(req);
    app->location = cfg->get_location(app->server, req);
    app->req = req;
    app->cgi = cgi;
    app->path_info_ = app->location->root() + "/" + app->req->tmp_path_info();
    return (app);
}

Response* GetCGIApplication::make_response()
{
    DEBUG("GetCGIApplication::make_response()");
    File *file = this->get_requested_file();
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
        ERROR("GetCGIApplication::make_response(): Neither file nor directory");
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

//Response* GetCGIApplication::make_response()
//Response* GetCGIApplication::make_response()
WebservCgiEvent *GetCGIApplication::cgi_event()
{
    cout << " get this->cgi_event_ No.0 = " << this->cgi_event_ << endl;
    return (this->cgi_event_);

}
/*
void GetCGIApplication::set_path_info(std::string const &path_info)
{
    this->path_info_ = path_info;
}

std::string &GetCGIApplication::get_path_info()
{
    return (this->path_info_);
}

std::string const &GetCGIApplication::path_info() const
{
    return (this->path_info_);
}
*/


