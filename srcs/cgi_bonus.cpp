#include "cgi.hpp"
#include "split.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

using std::cout;
using std::endl;
using std::map;
using std::string;

extern char** environ;

CGI::CGI() {}

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
        close(stdin_fd[1]);
        close(stdout_fd[0]);
        close(0);
        dup2(stdin_fd[0], 0);
        close(1);
        dup2(stdout_fd[1], 1);

    } else {
        close(stdin_fd[0]);
        close(stdout_fd[1]);

        *fd_in = stdin_fd[1];
        *fd_out = stdout_fd[0];

        char buf[1024] = {"12345\n\n890"};
        write(stdin_fd[1], buf, 7);
    }
    return pid;
}

int CGI::fork_parse(Request& req)
{
    string path = req.get_path();
    this->parse_cgi_file(path, req);

    if (req.get_method() == POST) {
        this->parse_args(req);
    }
    this->parse_args(this->query);

    std::map<string, string>::iterator ite = this->args.begin();
    std::map<string, string>::iterator end = this->args.end();
    for (; ite != end; ite++) {
        cout << "keys:" << ite->first << ", value:" << ite->second << endl;
    }

    char* argv[3];
    const char* filepath = this->cgi_filepath.c_str();
    char arg[1025];
    size_t i = 0;
    while (filepath[i]) {
        arg[i] = filepath[i];
        i++;
    }
    arg[i] = filepath[i];
    char python[] = "/home/sano/work/python/venv/DefaultVenv/bin/python3";
    char test_filepath[] = "/home/sano/work/42/webserv/github/webserv/srcs/dir/test.py";
    argv[0] = python;
    argv[1] = arg;
    argv[2] = NULL;
    argv[1] = test_filepath;

    int fd_in;
    int fd_out;
    int pid = make_thread(&fd_in, &fd_out);
    if (pid < 0) {
        cout << "Error" << endl;
    } else if (pid == 0) {
        int rval = execve(python, argv, environ);
        cout << "exe:" << this->exe_command.c_str() << endl;
        cout << "argv_p:" << argv[0] << endl;
        cout << "envirton[0]:" << environ[0] << endl;
        cout << "envirton[1]:" << environ[1] << endl;
        cout << "rval:" << rval << endl;
    }
    exit(1);
}
else if (pid > 0)
{
    // int wstatus;
    string tmp_str = "c++ test \nstdin\n";
    const char* tmp = tmp_str.c_str();
    write(fd_in, tmp, 12);
    this->result_fd = fd_out;

    char buf[1024];
    read(fd_out, buf, 10);
    // cout << "set last fd=" << this->result_fd << ", test read=" << buf << endl;
}
return (pid);
}

void CGI::parse_args(Request& req)
{
    (void)req;
}

int CGI::get_result_fd()
{
    return (this->result_fd);
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

void CGI::parse_cgi_file(string& uri, Request& req)
{
    (void)req;
    // todo will refer to config data
    std::map<string, string> cgi_map;
    cgi_map.insert(std::make_pair(".rb", "ruby"));
    cgi_map.insert(std::make_pair(".py", "python3"));
    cgi_map.insert(std::make_pair(".php", "php"));
    cgi_map.insert(std::make_pair(".pl", "perl"));

    cout << "uri origin:" << uri << endl;
    size_t pos = uri.find("?");
    string path;
    if (pos != std::string::npos) {
        this->query = uri.substr(pos + 1);
        path = uri.substr(0, pos);
    } else {
        this->query = "";
        path = uri;
    }

    pos = path.rfind(".");
    std::string extension = path.substr(pos);
    cout << "extention:" << extension << endl;
    if (cgi_map.count(extension) <= 0) {
        return;
    }
    this->exe_command = cgi_map[extension];
    this->cgi_filepath = path;
    cout << "exe=" << this->exe_command << endl;
    cout << "cgi_file=" << this->cgi_filepath << endl;
}
