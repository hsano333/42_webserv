#include "cgi.hpp"
#include "http_exception.hpp"
#include "webserv_event.hpp"
#include <unistd.h>

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
        close(0);
        dup2(stdin_fd[0], 0);
        close(1);
        dup2(stdout_fd[1], 1);

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
            //return ite->second;
            //return ;
        }
        ite++;
    }
    WARNING("not CGI");
    //throw std::invalid_argument("not CGI");
    return (false);
}


ApplicationResult *CGI::execute(ConfigLocation const *location, Request const *req)
{
    DEBUG("CGI::execute_cgi()");
    //this->cgi_event_.set_is_cgi(true);
    DEBUG("GetCGIApplication::execute()");
    //string &execve_path = this->cgi_application_path;
    string const &exe_root = location->root();
    string const &file_path = req->requested_filepath();
    string const &query = req->req_line().uri().query();
    string const path_info = location->root() + "/" + req->tmp_path_info();
    string env_query = "QUERY_STRING:" + query;
    string env_path_info = "PATH_INFO:" + path_info;

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
    int pid = this->make_thread(&fd_in, &fd_out);
    if(pid < 0){
        ERROR("failre to create thread:");
        throw HttpException("500");
    }

    if (pid == 0) {
        int rval = execve(file_path.c_str(), argv, env);
        cout << "rval:" << rval << endl;
        std::exit(0);
    }

    printf("fd_in=%d, fd_out=%d\n", fd_in, fd_out);
    printf("cgi pid parent\n");
    printf("cgi pid parent\n");
    printf("cgi pid parent\n");
    printf("cgi pid parent\n");

    DEBUG("fd_out(for read)=" + Utility::to_string(fd_out));
    DEBUG("fd_in(for write)=" + Utility::to_string(fd_in));
    /*
    char buf[10];
    int size = read(fd_out, buf, 9);
    buf[9] = '0';
    DEBUG("application execute read size=" + Utility::to_string(size));
    DEBUG("application execute read test=" + string(buf));
    */

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


    ApplicationResult *result = ApplicationResult::from_fd(fd_in, fd_out, pid);
    //WebservCgiEvent *cgi_event_ = new WebservCgiEvent();
    //cout << "this->cgi_event_ No.1 = " << cgi_event_ << endl;
    //FileDiscriptor pid_fd = FileDiscriptor::from_int(pid);
    //FileDiscriptor cgi_pid = FileDiscriptor::from_int(pid);
    //FileDiscriptor cgi_fd_in = FileDiscriptor::from_int(fd_in);
    //FileDiscriptor cgi_fd_out = FileDiscriptor::from_int(fd_out);
    //cgi_event_->set_pid(cgi_pid);
    //cgi_event_->set_fd_in(cgi_fd_in);
    //cgi_event_->set_fd_out(cgi_fd_out);
    return (result);

    //cout << "this->cgi_event_ No.2 = " << this->cgi_event_ << endl;

}



/*
int execute(int fd_in, int_fd_out, int pid)
{


}
*/

