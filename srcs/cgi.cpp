#include "cgi.hpp"
#include "split.hpp"
#include "uri.hpp"
#include <iostream>
#include <map>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using std::cout;
using std::endl;
using std::map;
using std::cerr;
using std::string;

extern char** environ;

CGI::CGI(Request* req) : _epfd(-1), _pid(-1), _req(req), _exit_status(0), _result_cgi(NULL)
{
    this->fork_parse();
}

CGI::~CGI()
{
    cout << "CGI close fd" << endl;
    cout << "CGI close fd" << endl;
    cout << "CGI close fd" << endl;
    cout << "CGI close fd" << endl;
    cout << "CGI close fd" << endl;
    if (_result_fd){
        close(_result_fd);
    }
    if (_result_fd_in){
        close(_result_fd_in);
    }
    if (_epfd > 0){
        close(_epfd);
    }
    delete _result_cgi;
}

int make_thread(int* fd_in, int* fd_out)
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

bool CGI::check_wait_process()
{
    if (this->get_end_process()){
        return true;
    }
    int status = 0;
    int result = waitpid(this->_pid, &status, WUNTRACED | WNOHANG);
    if (result == this->_pid) {
        _end_process = true;
        _exit_status = 0;
        if (WIFEXITED(status)) {
            _exit_status = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            _exit_status = WTERMSIG(status) + 128;
        }
    } else if (result == 0) {
        _end_process = false;
    }
    return (_end_process);
}

void CGI::fork_parse()
{
    const URI& uri = this->_req->get_uri();
    uri.print_uri();
    const vector<string> &query_vec = uri.get_query();
    std::string query = "";
    for (size_t i=0;i<query_vec.size(); i++){
        if(query_vec[i].find("&") != std::string::npos){
            query = query_vec[i];
            break;
        }
    }
    Split querys(query, "&");
    char* argv[querys.size() + 2] = {NULL};
    argv[0] = const_cast<char*>(uri.get_filepath().c_str());

    std::vector<string> add_env;
    //HttpMethod method(this->_req->get_method());
    std::string req_method = "REQUEST_METHOD:" + this->_req->get_method().to_string();
    //std::string req_method = "REQUEST_METHOD:";
    add_env.push_back(req_method);
    char** env = Utility::cp_env(environ, add_env);

    for (size_t i = 1; i <= querys.size(); i++) {
        argv[i] = const_cast<char*>(querys[i - 1].c_str());
    }
    const char* cgi_filepath = uri.get_relative_filepath().c_str();

    int fd_in;
    int fd_out;
    int pid = make_thread(&fd_in, &fd_out);
    if (pid < 0) {
        cout << "Error" << endl;
    } else if (pid == 0) {
        //cout << "root:" <<  uri.get_root() << endl;
        chdir(uri.get_root().c_str());
        int rval = execve(cgi_filepath, argv, env);
        if (rval < 0) {
            // error
            // todo write error status code to fd;
            cerr << strerror(errno) << endl;
        }
        exit(1);
    } else if (pid > 0) {
        /*
        this->_epfd = epoll_create(this->_sockets.size());
        if (_epfd < 0) {
            cout << "Epoll Error:" << strerror(errno) << endl;
            return (false);
        }
        struct epoll_event ev;
        memset(&ev, 0, sizeof(struct epoll_event));
        ev.events = EPOLLIN;
        if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, fd_out, &ev) != 0) {
            cout << "Epoll Ctl Error:" << strerror(errno) << endl;
            return (false);
        }
        */

        this->_result_fd = fd_out;
        this->_result_fd_in = fd_in;
        this->_pid = pid;
    }
}

/*
int CGI::read_result(char* buf, int size)
{
    return (read(_result_fd, buf, size));
}
*/

void CGI::parse_args(Request& req)
{
    (void)req;
}

int CGI::get_result_fd()
{
    return (this->_result_fd);
}

RequestCGI* CGI::get_result()
{
    cout << "get_result()" << endl;
    cout << "get_result()" << endl;
    cout << "get_result()" << endl;
    cout << "get_result()" << endl;
    if (this->_result_cgi){
        return (this->_result_cgi);
    }
    this->_result_cgi = new RequestCGI(_result_fd);
    return (this->_result_cgi);
}

void CGI::parse_args(std::string& query)
{
    Split sp(query, "&");
    for (size_t i = 0; i < sp.size(); i++) {
        size_t pos = sp[i].find("=");
        if (pos != std::string::npos) {
            this->args.insert(std::make_pair(sp[i].substr(0, pos), sp[i].substr(pos + 1)));
        }
    }
}

Request* CGI::get_req()
{
    return (_req);
}

int CGI::get_exit_status()
{
    return (_exit_status);
}

bool CGI::get_end_process()
{
    return (_end_process);
}

int CGI::get_sock_fd()
{
    return (_end_process);
}


