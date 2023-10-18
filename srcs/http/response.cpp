#include "response.hpp"

#include <stdexcept>
#include <fstream>
#include <sys/stat.h>
#include <utility.hpp>
#include <fcntl.h>
#include "config.hpp"

Response::Response()
{
;
}

Response::~Response()
{
;
}

Response::Response(Response const &res)
{
    *this = res;
}

Response& Response::operator=(Response const &res)
{
    if (this == &res){
        return (*this);
    }
    this->status_code = res.status_code;
    return (*this);
}

Response* Response::from_status_code(StatusCode &code)
{
    Response *res = new Response();
    res->status_code = code;
    return (res);
}

Response* Response::from_file(std::string const &filepath)
{
    (void)filepath;
    Response *res = new Response();
    return (res);
}

Response* Response::from_directory(std::string const &filepath)
{
    (void)filepath;
    Response *res = new Response();
    return (res);
}

void Response::print_info()
{
    cout << "|-- Print Response --|" << endl;
    cout << "status code:" << this->status_code.to_string() << endl;
    cout << "|--------------------------|" << endl;
}

/*

Response::Response(Request *request)
    : SocketData(),
      _request(request),
      _auto_index(false),
      _file_path("./Makefile"),
      _code(0),
      _send_state(STILL_NOT_SEND),
      _fd(-1),
      _status_line("")
{
}


Response::Response( int code)
    : SocketData(),
      _request(NULL),
      _auto_index(false),
      _file_path(""),
      _code(code),
      _send_state(STILL_NOT_SEND),
      _fd(-1),
      _status_line("")
{
    _code = code;
    get_status_line();
    //_status_line = ("HTTP/1.1 " +  Utility::to_string(code) + " " + this->get_status_message(code));
}

void Response::set_status(int code)
{
    _code = code;
    change_status_line();
}

void Response::change_status_line()
{
    _status_line = ("HTTP/1.1 " +  Utility::to_string(_code) + " " + this->get_status_message(_code));
}

string& Response::get_status_line()
{
    if (_status_line == ""){
        _status_line = ("HTTP/1.1 " +  Utility::to_string(_code) + " " + this->get_status_message(_code));
    }
    return (_status_line);
}

Response::~Response() {
    if (_fd > 0){
        close(_fd);
    }
}

void Response::set_header(const std::string& key, const std::string& value)
{
    _headers[key] = value;
}

std::string Response::get_error()
{
    //_code = 500;
    set_status(500);
    return (Response::get_status_line() + "\r\nContent-Length: 0\r\n\r\n");
}


string &Response::get_filepath()
{
    return (_file_path);
}

bool Response::open_responsed_file()
{
    string &filepath = this->get_filepath();
    if (filepath != ""){
        this->_fd = open(filepath.c_str(), O_RDONLY);
        if (_fd < 0){
            return (false);
        }
    }else{
        this->_fd = -1;
    }
    return (true);
}
void Response::close_responsed_file()
{
    if (_send_state == SENT_BODY || _send_state == ERROR) {
        if (_fd > 0){
            close(_fd);
        }
        _fd = -1;
        _send_state = CLOSE;
    }
}

void Response::check_error()
{
    if (_err){
        return ;
    }

    string &status_line = get_status_line();
    if(status_line.size() >=  BUF_MAX){
        _err = true;
        return ;
    }
    size_t headers_size = Utility::get_map_str_size(_headers);
    if (headers_size >= BUF_MAX){
        _err = true;
        return ;
    }
    struct stat fileInfo;
    if (stat(_file_path.c_str(), &fileInfo) != 0){
        _err = true;
        return ;
    }
    if (_auto_index){
        if ((fileInfo.st_mode & (S_IFREG | S_IFDIR)) == false){
            _err = true;
            return ;
        }
    }else{
        if ((fileInfo.st_mode & S_IFREG &S_IFLNK) == false){
            _err = true;
            return ;
        }
    }
    if ((size_t)fileInfo.st_size >= Config::get_instance()->http->get_max_body_size()){
        _err = true;
        return ;
    }
    if ((fileInfo.st_mode & S_IRUSR) == false){
        _err = true;
        return ;
    }
}

ssize_t Response::get_data(char** data)
{
    if (_send_state == STILL_NOT_SEND) {
        check_error();
        if (_err){
            _code = 500;
            _headers.clear();
        }else if(_code <= 0){
            set_status(200);
        }
        string &status = get_status_line();
        *data = const_cast<char*>(&(status[0]));
        _send_state = SENT_STATUS_LINE;
        return (status.size());
    }else if (_send_state == SENT_STATUS_LINE){
        string tmp = "Content-Type: text/plain\r\nTransfer-Encoding: chunked\r\n";
        *data = const_cast<char*>(&(tmp[0]));
        _send_state = SENT_HEADER;
        return (tmp.size());

    }else if (_send_state == SENT_HEADER){
        int size = Utility::read_body_and_copy(_fd, data, BUF_MAX);
        if (size <= 0){
            _send_state = SENT_BODY;
        }
        return (size);
    }
    return (0);
}
*/

