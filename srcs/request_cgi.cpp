#include "request_cgi.hpp"
#include "no_request_line.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

RequestCGI::RequestCGI(int fd) : Request(fd, new ReadNormal()), _status_code(0), _has_extra_buf(true), _is_closed(false)
{
    cout << "RequestCGI open:" << fd << endl;
    cout << "RequestCGI open:" << fd << endl;
    cout << "RequestCGI open:" << fd << endl;
    cout << "RequestCGI open:" << fd << endl;
    exit(1);
    int cur_flags = fcntl(fd, F_GETFL, 0);
    cur_flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, cur_flags);
    this->parse();
    //_file_fd = fd;
}

RequestCGI::~RequestCGI() {
    if (this->_fd > 0){
        cout << "resultCGI close:" << this->_fd << endl;
        close(this->_fd);
    }
}

void RequestCGI::parse()
{
    /*
    try {
        parse_request_line();
    } catch (NoRequestLineException& e) {
        this->_buf.restart();
    }
    this->print_request();
    this->_headers.clear();
    parse_header_field();
    this->print_request();
    */
}

bool RequestCGI::exist_ContentTypeHeader()
{
    /*
    map<string, string>::iterator ite = this->_headers.find("content-type");
    map<string, string>::iterator end = this->_headers.end();
    if (ite == end) {
        return (false);
    }
    */
    return (true);
}

bool RequestCGI::exist_LocationHeader()
{
    /*
    map<string, string>::iterator ite = this->_headers.find("location");
    map<string, string>::iterator end = this->_headers.end();
    if (ite == end) {
        return (false);
    }
    */
    return (true);
}

bool RequestCGI::exist_ContentLengthHeader()
{
    /*
    map<string, string>::iterator ite = this->_headers.find("content-length");
    map<string, string>::iterator end = this->_headers.end();
    if (ite == end) {
        return (false);
    }
    */
    return (true);
}

bool RequestCGI::exist_StatusHeader()
{
    /*
    map<string, string>::iterator ite = this->_headers.find("status");
    map<string, string>::iterator end = this->_headers.end();
    if (ite == end) {
        return (false);
    }
    */
    return (true);
}

void RequestCGI::parse_request_line()
{
    /*
    string request_line = _buf.getline();
    if (request_line == _buf.last_str) {
        return;
    }
    Split request_line_words(request_line, " ");
    if (request_line_words.size() != 3) {
        throw NoRequestLineException();
    }
    std::cout << request_line_words << std::endl;
    Split::iterator request_line_words_it = request_line_words.begin();
    int status_code = Utility::to_int(*request_line_words_it);
    if (100 <= status_code && status_code < 1000){
        this->_status_code = status_code;
    }else{
        throw NoRequestLineException();
    }
    this->_status_str = Utility::trim_white_space(*(++request_line_words_it));
    */
}

void RequestCGI::set_status_code(int status_code)
{
    this->_status_code = status_code;
}

int RequestCGI::get_status_code()
{
    return (this->_status_code);
}

std::string &RequestCGI::get_status_str()
{
    return (this->_status_str);
}

Port &RequestCGI::get_header_line()
{
    //return (_buf.getline());
    return (this->port);
}

void RequestCGI::get_concat_header(string &header)
{
    /*
    map<string,string>::iterator ite = this->_headers.begin();
    map<string,string>::iterator end = this->_headers.end();
    for(; ite != end ; ite++){
        header += ite->first + ": " + ite->second + "\r\n";
    }
    */
    cout << "header concat: " << header << endl;
}

int RequestCGI::get_extra_buf(char **data)
{
    (void)data;
    //_has_extra_buf = false;
    //return (this->_buf.get_extra_buf_for_CGI(data));
    return (1);
}

bool RequestCGI::has_extra_buf()
{
    return (false);
    //return (_has_extra_buf);
}


/*
int get_fd_tmp()
{
    std::string path = "/home/sano/work/42/webserv/github/webserv/Makefile";
    //int fd = open(path.c_str(), O_RDONLY);
    int fd = 0;
    return fd;
}
*/

       #include <string.h>

#include <stdio.h>
int RequestCGI::read_result(char** buf, int size)
{
    exit(1);
    //cout << "read_result: fd" << _fd << ", _file_fd:" << _file_fd << endl;
    return (Utility::read_body_and_copy(_fd, buf, BUF_MAX));
    char *tmp = *buf;
    //if (_file_fd == 0){
        //_file_fd = get_fd_tmp();
        //_file_fd = _fd;
    //}
    int chunk_size = 30;
    int read_size = read(this->_fd, &(tmp[chunk_size]), size - chunk_size);
    if (read_size <= 0){
        //cerr << "read_result close:" << _file_fd << endl;
        //close(_file_fd);
        cout << strerror(errno) << endl;
        return 0;
    }
    string size_str = Utility::to_hexstr(read_size);
    size_str += "\r\n";
    size_t len = size_str.size();
    Utility::memcpy(&(tmp[chunk_size-len]), size_str.c_str(), len);
    *buf= &(tmp[chunk_size-len]);
    return (read_size+len);
}

bool RequestCGI::is_cgi()
{
    cout << "Result CGI read_result" << endl;
    cout << "Result CGI read_result" << endl;
    cout << "Result CGI read_result" << endl;
    cout << "Result CGI read_result" << endl;
    return (false);
}

bool RequestCGI::is_full_body_loaded() const
{
    //if (this->_cgi)

    if(_is_closed == true){
        return (true);
    }
    return (false);


}
