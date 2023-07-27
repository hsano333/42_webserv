#ifndef CGI_HPP
#define CGI_HPP
#include "request.hpp"
#include "request_cgi.hpp"
#include "utility.hpp"
#include <map>
#include <string>

enum E_CGI {
    PHP,
    RUBY,
    PYTHON2,
    PYTHON3,
    PERL,
};

class CGI
{
  public:
    CGI(Request* req);
    ~CGI();
    //CGI(CGI& const cgi);
    //CGI& operator=(CGI& const cgi);
    void fork_parse();
    int get_sock_fd();
    int get_result_fd();
    bool check_wait_process();
    Request* get_req();
    int get_exit_status();
    bool get_end_process();
    RequestCGI* get_result();

  private:
    int _epfd;
    void parse_args(Request& req);
    void parse_args(std::string& uri);
    std::string _cgi_filepath;
    std::string _root;
    std::string _query;
    std::string _path_info;
    std::map<std::string, std::string> args;
    int _result_fd;
    int _result_fd_in;
    bool _end_process;
    int _pid;
    // int _fd_out;
    Request* _req;
    int _exit_status;
    RequestCGI *_result_cgi;
};

#endif
