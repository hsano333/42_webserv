#include "cgi.hpp"
#include "config_cgi.hpp"
#include "http_exception.hpp"
#include "webserv_event.hpp"
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

CGI::CGI()
{
;
}

CGI::~CGI()
{
;
}

int CGI::make_thread(int* fd_in, int* fd_out)
{
    int stdin_fd[2];
    int stdout_fd[2];
    int rval_in = pipe(stdin_fd);
    if (rval_in < 0) {
        return -1;
    }
    int rval_out = pipe(stdout_fd);
    if (rval_out < 0) {
        return -1;
    }
    pid_t pid = fork();
    if (pid == 0) {
        close(stdin_fd[1]);
        close(stdout_fd[0]);
        dup2(stdin_fd[0], 0);
        close(stdin_fd[0]);
        dup2(stdout_fd[1], 1);
        close(stdout_fd[1]);

    } else if(pid > 0) {
        DEBUG("Child PID:" + Utility::to_string(pid));
        close(stdin_fd[0]);
        close(stdout_fd[1]);

        *fd_in = stdin_fd[1];
        *fd_out = stdout_fd[0];
        DEBUG("cgi fd_in fd:" + Utility::to_string(*fd_in));
        DEBUG("cgi fd_out fd:" + Utility::to_string(*fd_out));
    }
    return pid;
}


bool CGI::check_cgi_application_path(const Request *req, const ConfigLocation *location) const
{
    (void)req;
    (void)location;
    const ConfigLimit *limit = location->limit();

    if (req->is_file() == false){
        WARNING("not CGI");
        return (false);
    }

    if (limit == NULL){
        WARNING("not CGI");
        return (false);
    }

    const ConfigCgi *config_cgi = limit->cgi();
    if (config_cgi == NULL){
        WARNING("not CGI");
        return (false);
        throw std::invalid_argument("not CGI");
    }

    const std::map<std::string, std::string> &ext = config_cgi->get_extensions();
    std::map<std::string, std::string>::const_iterator ite = ext.begin();
    std::map<std::string, std::string>::const_iterator end = ext.end();

    std::string const filepath = req->requested_filepath();
    size_t pos = filepath.rfind("/");
    std::string filename = filepath.substr(pos+1);

    while(ite != end){
        const std::string &file_ext = ite->first;
        pos = filename.rfind(file_ext);
        if (pos != std::string::npos && (pos + file_ext.size() == filename.size())){
            MYINFO("CGI::is_cgi execve:" + ite->second);
            MYINFO("CGI::is_cgi filepath:" + filepath);
            return (true);
        }
        ite++;
    }
    WARNING("not CGI");
    return (false);
}

bool check_extension(std::string const &path, ConfigCgi const *config_cgi)
{

    size_t pos = path.rfind(".");
    if(pos == std::string::npos){
        return (false);
    }
    string extension = path.substr(pos);
    DEBUG("check extension:" + extension);
    std::map<std::string, std::string>::const_iterator exe_path = config_cgi->get_extensions().find(extension);
    if(exe_path == config_cgi->get_extensions().end()){
        DEBUG("Error check extension:" + extension);
        return (false);
    }
    
    return (true);

}

ApplicationResult *CGI::execute(WebservEntity *entity, const Method &method)
{
    Request *req = entity->request();
    Config const *cfg = entity->config();
    ConfigServer const *server = cfg->get_server(req);
    ConfigLocation const *location = cfg->get_location(server, req);
    req->set_path_info(location->root());

    string const &file_path = req->requested_filepath();
    string const &query = req->req_line().uri().query();
    string const script_file_name = file_path.substr(file_path.rfind("/"));

    if(!((Utility::is_executable_file(file_path) && Utility::is_readable_file(file_path)))){
        ERROR("cgi file is not executable, or not readable:" + file_path);
        throw HttpException("403");
    }

    DEBUG("GetCGIApplication::execute()");

    string const document_root = "DOCUMENT_ROOT=" + location->root();
    string const gateway_interface= "GATEWAY_INTERFACE=1.1";
    string const path_translated = location->root() + "/" + req->tmp_path_info();
    string const remote_host = "REMOTE_HOST=" + req->header().get_host();
    string const request_method= "REQUEST_METHOD=" + method.to_string();
    string const script_name = "SCRIPT_NAME=" + script_file_name;
    string const server_name = "SERVER_NAME=" + server->server_name();
    string const server_port = "SERVER_PORT=" + server->listen().to_string();
    string const server_protocol = "SERVER_PROTOCOL=" + req->req_line().version().to_string();
    string const server_software = "SERVER_SOFTWARE=Webserv 0.0.1";
    string const remote_USER = "REMOTE_USER=" + req->auth();

    string const content_length = "CONTENT_LENGTH=" + req->header().get_content_length_str();
    string const content_type = "CONTENT_TYPE=" + req->header().get_content_type();
    string const env_query = "QUERY_STRING=" + query;
    string const path_info = "PATH_INFO=" + location->root() + "/" + req->tmp_path_info();
    string const cookie = "HTTP_COOKIE=" + req->header().cookie();
    DEBUG("CGI path_info:" + path_info);
    DEBUG("CGI file_path:" + Utility::to_string(file_path));

    //対応しない
    ////string const remote_addr = "REMOTE_ADDR=";
    //string const remote_IDENT = "REMOTE_IDENT=";

    char* argv[2] = {NULL};
    argv[0] = const_cast<char*>(file_path.c_str());


    char *env[30] = {NULL};
    env[0] = const_cast<char*>(document_root.c_str());
    env[1] = const_cast<char*>(gateway_interface.c_str());
    env[2] = const_cast<char*>(path_translated.c_str());
    env[3] = const_cast<char*>(remote_host.c_str());
    env[4] = const_cast<char*>(request_method.c_str());
    env[5] = const_cast<char*>(script_name.c_str());
    env[6] = const_cast<char*>(server_port.c_str());
    env[7] = const_cast<char*>(server_protocol.c_str());
    env[8] = const_cast<char*>(server_software.c_str());
    env[9] = const_cast<char*>(remote_USER.c_str());
    env[10] = const_cast<char*>(content_length.c_str());
    env[11] = const_cast<char*>(content_type.c_str());
    env[12] = const_cast<char*>(env_query.c_str());
    env[13] = const_cast<char*>(path_info.c_str());
    env[14] = const_cast<char*>(server_name.c_str());
    env[15] = const_cast<char*>(cookie.c_str());

    int fd_in;
    int fd_out;
    int pid = this->make_thread(&fd_in, &fd_out);
    if(pid < 0){
        ERROR("failre to create thread:");
        throw HttpException("500");
    }


    if (pid == 0) {
        int rval = execve(file_path.c_str(), argv, env);
        if(rval < 0){
            exit(1);
        }
        std::exit(0);
    }

    ApplicationResult *result = ApplicationResult::from_fd(entity->fd(), fd_in, fd_out, pid);
    return (result);
}

