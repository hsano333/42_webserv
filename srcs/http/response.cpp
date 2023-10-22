#include "response.hpp"

#include <stdexcept>
#include <fstream>
#include <sys/stat.h>
#include <utility.hpp>
#include <fcntl.h>
#include "config.hpp"
#include "error_file.hpp"

Response::Response() :
    file(NULL),
    is_redirect(false),
    send_state(STILL_NOT_SEND),
    exist_body_(false)
{
    this->headers.insert(std::make_pair("Server", WEBSERV_VERSION));
    this->headers.insert(std::make_pair("Date", Utility::time_to_string()));
}

Response::~Response()
{
    delete this->file;
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
    delete this->file;

    this->status_code = res.status_code;
    this->file = res.file;
    this->is_redirect = res.is_redirect;
    this->send_state = res.send_state;
    this->status_line = res.status_line;
    this->exist_body_ = res.exist_body_;
    return (*this);
}

Response* Response::from_error_status_code(StatusCode &code)
{
    Response *res = new Response();
    res->status_code = code;
    res->file = ErrorFile::from_status_code(code);
    res->exist_body_ = true;
    return (res);
}

Response* Response::from_file(File *file)
{
    Response *res = new Response();
    res->file = file;
    if (file->can_read()){
        res->status_code = StatusCode::from_int(200);
    }else{
        res->status_code = StatusCode::from_int(406);
    }
    return (res);
}

void Response::set_exist_body(bool flag)
{
    this->exist_body_ = flag;
}

void Response::add_header(std::string const &key, std::string const &value)
{
    this->headers.insert(std::make_pair(key, value));
}

int Response::open_file()
{
    if (this->file){
        return (this->file->open());
    }
    return 0;
}

int Response::close_file()
{
    if (this->file){
        return (this->file->close());
    }
    return 0;
}

void Response::make_status_line()
{
    this->status_line = ("HTTP/1.1 " +  status_code.to_string() + " " + status_code.message() + "\r\n");
}

void Response::make_header_line()
{
    //this->header_line = ("HTTP/1.1 " +  status_code.to_string() + " " + status_code.message());
    if (this->exist_body_ ){
        if (this->file->is_chunk()){
            this->header_line = "Transfer-Encoding: chunked\r\n";
        }else{
            this->header_line = "Content-Length: " + Utility::to_string(this->file->size()) + "\r\n";
        }
    }else{
        this->header_line = "Content-Length: 0\r\n";
    }
    std::map<std::string, std::string>::iterator ite = this->headers.begin();
    std::map<std::string, std::string>::iterator end = this->headers.end();
    while(ite != end){
        this->header_line += ite->first + ": " + ite->second + "\r\n";
        ite++;
    }
    this->header_line += "\r\n";
}

int Response::read_body_and_copy(char** dst, size_t size)
{
    int result = (this->file->read(dst, size));
    cout << "read test:" << *dst << "], size=" << result << endl;
    return (result);
    /*
    //printf("No.3 data=%p\n", *dst);
    char *tmp = *dst;
    //printf("No.4 data=%p\n", tmp);
    int chunk_size = 20;
    char *tmp2 = &(tmp[chunk_size]);
    int read_size = this->file->read(&(tmp2), size - chunk_size);
    if (read_size <= 0){
        return 0;
    }
    // chunkサイズは16進数
    string size_str = CRLF;
    size_str += Utility::to_hexstr(read_size);
    size_str += CRLF;
    size_t len = size_str.size();
    Utility::memcpy(&(tmp[chunk_size-len]), size_str.c_str(), len);
    *dst = &(tmp[chunk_size-len]);
    */
    //return (read_size+len);
}

int Response::read_body_and_copy_chunk(char** dst, size_t size)
{
    char *tmp = *dst;
    int chunk_size = 20;
    char *tmp2 = &(tmp[chunk_size]);
    int read_size = this->file->read(&(tmp2), size - chunk_size);
    //int read_size = this->file->read(&(tmp[chunk_size]), size - chunk_size);

    // chunkサイズは16進数
    //string size_str = CRLF;
    std::string size_str = Utility::to_hexstr(read_size);
    size_str += CRLF;
    size_t len = size_str.size();
    Utility::memcpy(&(tmp[chunk_size-len]), size_str.c_str(), len);
    *dst = &(tmp[chunk_size-len]);
    if (read_size > 0){
        //転送終了痔だけ改行を付けない
        tmp2[0] = '\r';
        tmp2[1] = '\n';
        read_size += 2;
    }

    return (read_size+len);
}

ssize_t Response::get_data(char** data)
{
    //printf("No.2 data=%p\n", *data);
    if (this->send_state == STILL_NOT_SEND) {
        this->make_status_line();
        *data= const_cast<char*>(&(this->status_line[0]));
        this->send_state = SENT_STATUS_LINE;
        return (this->status_line.size());
    }else if (this->send_state == SENT_STATUS_LINE){
        this->make_header_line();
        *data= const_cast<char*>(&(this->header_line[0]));
        this->send_state = SENT_HEADER;
        return (this->header_line.size());
    }else if (this->send_state == SENT_HEADER && this->exist_body_){

        int size;
        if (this->file->is_chunk()){
            size = this->read_body_and_copy_chunk(data, MAX_READ_SIZE);
            if (size <= 7){
                this->send_state = SENT_BODY;
            }
        }else{
            size = this->read_body_and_copy(data, MAX_READ_SIZE);
            cout << "not chunk size=" << size << endl;
            if (size <= 0){
                this->send_state = SENT_BODY;
            }
        }
        return (size);
    }
    //}else if (this->send_state != CLOSE){
        //cout << "CLOSE " << endl;
        //(*data)[0] = '\r';
        //(*data)[1] = '\n';
        //this->send_state = CLOSE;
        //return (2);
    //}
    cout << "END" << endl;
        //Utility::memcpy(&((*data)[1]), CRLF2, 4);
        //(*data)[3] = '\r';
        //(*data)[4] = '\n';
        //this->send_state = CLOSE;
        //return (5);
    //}
    return (0);
}



/*
Response* Response::from_file(std::string const &filepath)
{
    (void)filepath;
    Response *res = new Response();
    res->file = file

    return (res);
}

Response* Response::from_directory(std::string const &filepath)
{
    (void)filepath;
    Response *res = new Response();
    return (res);
}
*/

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
      this->send_state(STILL_NOT_SEND),
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
      this->send_state(STILL_NOT_SEND),
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

bool Response::open_file()
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
void Response::close_file()
{
    if (this->send_state == SENT_BODY || this->send_state == ERROR) {
        if (_fd > 0){
            close(_fd);
        }
        _fd = -1;
        this->send_state = CLOSE;
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
    if (this->send_state == STILL_NOT_SEND) {
        check_error();
        if (_err){
            _code = 500;
            _headers.clear();
        }else if(_code <= 0){
            set_status(200);
        }
        string &status = get_status_line();
        *data = const_cast<char*>(&(status[0]));
        this->send_state = SENT_STATUS_LINE;
        return (status.size());
    }else if (this->send_state == SENT_STATUS_LINE){
        string tmp = "Content-Type: text/plain\r\nTransfer-Encoding: chunked\r\n";
        *data = const_cast<char*>(&(tmp[0]));
        this->send_state = SENT_HEADER;
        return (tmp.size());

    }else if (this->send_state == SENT_HEADER){
        int size = Utility::read_body_and_copy(_fd, data, BUF_MAX);
        if (size <= 0){
            this->send_state = SENT_BODY;
        }
        return (size);
    }
    return (0);
}
*/

