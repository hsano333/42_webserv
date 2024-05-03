#include "cgi.hpp"
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
        return 0;
    }
    int rval_out = pipe(stdout_fd);
    if (rval_out < 0) {
        return 0;
    }
    pid_t pid = fork();
    if (pid == 0) {
        cout << "children :close1:" << stdin_fd[1] << ", close2:" << stdout_fd[0] << endl;
        close(stdin_fd[1]);
        close(stdout_fd[0]);
        dup2(stdin_fd[0], 0);
        close(stdin_fd[0]);
        dup2(stdout_fd[1], 1);
        close(stdout_fd[1]);

    } else {
        cout << "parent:close1:" << stdin_fd[0] << ", close2:" << stdout_fd[1] << endl;
        close(stdin_fd[0]);
        close(stdout_fd[1]);

        *fd_in = stdin_fd[1];
        *fd_out = stdout_fd[0];
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
        //throw std::invalid_argument("not CGI");
    }

    if (limit == NULL){
        WARNING("not CGI");
        return (false);
        //throw std::invalid_argument("not CGI");
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


ApplicationResult *CGI::execute(ConfigLocation const *location, Request const *req)
{
    DEBUG("GetCGIApplication::execute()");
    //string const &exe_root = location->root();
    string const &file_path = req->requested_filepath();
    string const &query = req->req_line().uri().query();
    string const path_info = location->root() + "/" + req->tmp_path_info();
    DEBUG("query:" + query);
    string env_query = "QUERY_STRING=" + query;
    DEBUG("env_query:" + env_query);
    string env_path_info = "PATH_INFO=" + path_info;

    char* argv[2] = {NULL};
    argv[0] = const_cast<char*>(file_path.c_str());


    //string env1 = "PATH_INFO=/abc";
    //string env2 = "QUERY_STRING=abc=123&b=12aa";
    char *env[3] = {NULL};
    //env[0] = const_cast<char*>(env_query.c_str());
    //env[0] = const_cast<char*>(env1.c_str());
    //env[1] = const_cast<char*>(env2.c_str());
    env[0] = const_cast<char*>(env_query.c_str());
    env[1] = const_cast<char*>(env_path_info.c_str());
    env[2] = NULL;
    /*
    char* env[3] = {NULL};
    env[0] = const_cast<char*>(env_query.c_str());
    env[1] = const_cast<char*>(env_path_info.c_str());
    */

    int fd_in;
    int fd_out;
    int pid = this->make_thread(&fd_in, &fd_out);
    if(pid < 0){
        ERROR("failre to create thread:");
        throw HttpException("500");
    }

    /*
    int env_i = 0;
    while(environ[env_i]){
        cout << environ[env_i++] << endl;
    }
    */

    if (pid == 0) {
        //int rval = execve(file_path.c_str(), argv, environ);
        int rval = execve(file_path.c_str(), argv, env);
        cout << "rval:" << rval << endl;
        std::exit(0);
    }

    MYINFO("fd_out(for read)=" + Utility::to_string(fd_out));
    MYINFO("fd_in(for write)=" + Utility::to_string(fd_in));

    ApplicationResult *result = ApplicationResult::from_fd(fd_in, fd_out, pid);
    return (result);
}

