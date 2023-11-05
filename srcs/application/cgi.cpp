#include "cgi.hpp"
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


void CGI::check_cgi_application_path(const Request *req, const ConfigLocation *location) const
{
    (void)req;
    (void)location;
    const ConfigLimit *limit = location->limit();

    if (req->is_file() == false){
        WARNING("not CGI");
        throw std::invalid_argument("not CGI");
    }

    if (limit == NULL){
        WARNING("not CGI");
        throw std::invalid_argument("not CGI");
    }

    const ConfigCgi *config_cgi = limit->cgi();
    if (config_cgi == NULL){
        WARNING("not CGI");
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
            //return ite->second;
            return ;
        }
        ite++;
    }
    WARNING("not CGI");
    throw std::invalid_argument("not CGI");
}



/*
int execute(int fd_in, int_fd_out, int pid)
{


}
*/

