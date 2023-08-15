#include "request.hpp"
#include "config.hpp"
#include "split.hpp"
#include "utility.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "no_request_line.hpp"
#include "global.hpp"
#include "get_method.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::tolower;
using std::vector;

#define BODY_TMP_DIRECTORY_PATH "/tmp/webserv_body_tmp/"

Request::Request(int fd_) : SocketData(),
                              _fd(fd_),
                              _method(NULL),
                              _buf_max(false),
                              _buf_pos(0),
                              _end_status_line(0),
                              _state(STILL_NOT_LOAD),
                              _iread(new ReadSocket()),
                              _uri()
                              //_content_length(0)

                              //_buf(this->_fd),
                              //_method(HttpMethod::INVALID),
                              //_header_read_complete(false),
                              //_content_length(0),
                              //_err_line("")
{
}


Request::Request(int fd_, IRead* iread) : SocketData(),
                              _fd(fd_),
                              _method(NULL),
                              _buf_max(false),
                              _buf_pos(0),
                              _end_status_line(0),
                              _state(STILL_NOT_LOAD),
                              _iread(iread),
                              _uri()
                              //_content_length(0)
{
}

Request::~Request()
{
    _uri.parse(this);
}

void Request::parse()
{

}

int Request::read_fd()
{
    int read_size = MAX_BUF - _buf_pos;
    if(read_size < MAX_READ){
        _err = true;
        this->_buf_max = true;
        return (-1);
    }
    int size = (this->_iread->iread(_fd, &(this->_buf[_buf_pos]), MAX_READ));
    _buf_pos += size;
    this->_buf[_buf_pos] = '\0';
    return (size);
}

void Request::parse_status_line(char *crlf, int read_size)
{
    (void)read_size;
    crlf[0] = '\0';
    this->_end_status_line = (crlf - _buf) + 2;

    Split sp(this->_buf, " ");
    if (sp.size() != 3) {
        throw NoRequestLineException();
    }
    std::transform(sp[0].begin(), sp[0].end(), sp[0].begin(), static_cast<int (*)(int)>(std::toupper));
    //GetMethod* test = new GetMethod();
    //(void)test;
    _method = Method::make_method(sp[0]);
    //_method = new GetMethod();
    _path = Utility::delete_duplicated_slash(Utility::trim_white_space(sp[1]));
    //_uri.set_raw(sp[1]);
    //_uri = URI(sp[1]);
    _version = Utility::trim_white_space(sp[2]);
}

void Request::parse_header()
{
    this->body_pos = Utility::strnstr(&(this->_buf[this->_end_status_line]), "\r\n\r\n", this->_buf_pos - this->_end_status_line+1);
    if (this->body_pos){
        this->body_pos = '\0';
        if (&(this->_buf[this->_buf_pos]) > this->body_pos + 2){
            this->body_pos += 2;
            _state = LOADED_STATUS_HEADER;
        }else{
            this->body_pos = NULL;
            _state = END_LOAD;
        }
        this->body_pos += 2;
        _headers.load_data(&(this->_buf[this->_end_status_line]));
    }
}

void Request::load_body()
{
    if (this->_method->is_need_body() == false){
        _state = LOADED_STATUS_BODY;
        return ;
    }

    if (this->body_pos){
        this->_body_size = &(this->_buf[this->_buf_pos]) - this->body_pos;
        if (this->_body_size < 0){
            this->_err = true;
            return ;
        }
        if (this->_headers.get_content_length() <= (size_t)this->_body_size){
            _state = LOADED_STATUS_BODY;
            //_state = END_LOAD;
        }else{
            Utility::memcpy(this->_buf, this->body_pos, this->_body_size);
            this->body_pos = this->_buf;
            this->_buf_pos = this->_body_size;
        }
    }else{
        //this->_body_size

    }
}

void Request::read_up_to_header()
{
    if (_state == LOADED_STATUS_BODY){
        return ;
    }
    int size = read_fd();
    int sum_size = size;
    while(size > 0)
    {
        char *crlf = Utility::strnstr(&(this->_buf[this->_end_status_line]), "\r\n", this->_buf_pos - this->_end_status_line+1);
        if (crlf){
            if (_state == STILL_NOT_LOAD){
                this->parse_status_line(crlf, size);
                _state = LOADED_STATUS_LINE;
            }
            if (_state == LOADED_STATUS_LINE){
                this->parse_header();
            }
            if (_state == LOADED_STATUS_HEADER){
                this->load_body();
            }
        }
        size = read_fd();
        sum_size += size;
    }
}

REQUEST_STATE Request::get_state()
{
    return (_state);
}

void Request::set_port(Port &port)
{
    this->port = port;
}

const Port& Request::get_port() const
{
    return port;
}

const std::string& Request::get_host() const
{
    return _host;
}

const std::string& Request::get_path() const
{
    return _path;
}

const Method& Request::get_method() const
{
    return *_method;
}

const URI& Request::get_uri() const
{
    return (_uri);
}

bool Request::is_cgi()
{
    return (false);
}

void Request::print_request() const
{
    cout << "|-- Print Request Header --|" << endl;
    cout << " fd: " << _fd << endl;
    cout << " method: " << _method << endl;
    cout << " version: " << _version << endl;

    //cout << " headers size: " << _headers.size() << endl;
    cout << " path: " << _path << endl;
    this->_headers.print();
    //cout << " path dep: " << get_path_list().size() << endl;
    //cout << " content-type: " << _content_type << endl;
    //cout << " content-length: " << _content_length << endl;
    //cout << " transfer-encoding: " << _transfer_encoding.get_str() << endl;
    //map<string, string>::const_iterator ite = _headers.begin();
    //map<string, string>::const_iterator end = _headers.end();
    //for (; ite != end; ite++)
        ////cout << " _headers:" << (*ite).first << ": " << (*ite).second << endl;
    cout << "|--------------------------|" << endl;
}


//Request::Request(int fd_)
//    : SocketData(),
//      _fd(fd_),
//      _buf(this->_fd),
//      _method(HttpMethod::INVALID),
//      _header_read_complete(false),
//      _content_length(0),
//      _err_line("")
//{
//    this->read_to_header();
//    //this->parse();
//    const vector<Server const*> server_list = Config::get_instance()->http->server;
//}
//Request::Request(int fd_, string& port)
//    : SocketData(),
//      _fd(fd_),
//      _buf(this->_fd),
//      _method(HttpMethod::INVALID),
//      _content_length(0),
//      _err_line(""),
//      port(port)
//{
//    this->parse();
//    const vector<Server const*> server_list = Config::get_instance()->http->server;
//}
//
////This constructor is to initialize ReqeustCGI that is children of Reqeust
//Request::Request(int fd_, IRead* iread)
//    : SocketData(),
//      _fd(fd_),
//      _buf(this->_fd, iread),
//      _method(HttpMethod::INVALID),
//      _content_length(0),
//      _err_line("")
//{
//}
//
//Request::~Request() {
//    //if (_fd > 0){
//        //close(_fd);
//        //this->_fd = 0;
//    //}
//}
//
//
//
//
//void Request::read_to_header()
//{
//
//    //int size = this->read(this->_buf, BUF_MAX);
//
//    string request_line = _buf.getline();
//    if (request_line == _buf.last_str) {
//        return;
//    }
//    _except_body += "test";
//}
//
//void Request::parse()
//{
//    parse_request_line();
//    parse_header_field();
//
//    parse_content_length();
//    _transfer_encoding = TransferEncoding(_headers["transfer-encoding"]);
//    _content_type = ContentType(_headers);
//    ByteVector tmp_loaded_packet_body = read_body();
//
//    // tmpファイルに保存する場合はここにif文を作り分岐させる #39
//    _loaded_packet_body = tmp_loaded_packet_body;
//}
//
//void Request::parse_request_line()
//{
//    string request_line = _buf.getline();
//    if (request_line == _buf.last_str) {
//        return;
//    }
//    std::cout << "request line: " << request_line << std::endl;
//    Split request_line_words(request_line, " ");
//    if (request_line_words.size() != 3) {
//        throw NoRequestLineException();
//    }
//    std::cout << request_line_words << std::endl;
//    Split::iterator request_line_words_it = request_line_words.begin();
//    _method = HttpMethod(*request_line_words_it);
//    _path = Utility::trim_white_space(*(++request_line_words_it));
//    _version = Utility::trim_white_space(*(++request_line_words_it));
//}
//
//void Request::parse_header_field()
//{
//    string key;
//    string value;
//    std::string::size_type split_pos;
//    string line = _buf.getline();
//    while ((line != _buf.last_str)) {
//        split_pos = line.find(":");
//        if (split_pos == string::npos || line.size() <= 0) {
//            break;
//        }
//        key = line.substr(0, split_pos);
//        key = Utility::trim_white_space(key);
//        if (key.size() < 2) {
//            break;
//        }
//        std::transform(key.begin(), key.end(), key.begin(), static_cast<int (*)(int)>(std::tolower));
//        value = line.substr(split_pos + 1);
//        value = Utility::trim_white_space(value);
//        this->_headers.insert(make_pair(key, value));
//        line = _buf.getline();
//    }
//
//    // uriに関する情報をひとまとめにしたクラスに格納する
//    try {
//        _uri = URI(this);
//        // テスト用　後で消す
//        _uri.print_uri();
//    } catch (std::invalid_argument& e) {
//        cout << "URI Error" << endl;
//        cout << e.what() << endl;
//    }
//    this->_header_read_complete = true;
//}
//
//void Request::parse_content_length()
//{
//    string size_str = _headers["content-length"];
//    ssize_t size = -1;
//    if (size_str.size() > 10) {
//        cout << "Error: exceed BODY SIZE MAX" << endl;
//    }
//    if (size_str != "") {
//        std::stringstream ss;
//        ss << size_str;
//        ss >> size;
//    }
//    this->_content_length = size;
//}
//
//HttpMethod Request::get_method() const
//{
//    return this->_method;
//}
//
//const string& Request::get_version() const
//{
//    return (this->_version);
//}
//
//const map<string, string>& Request::get_headers() const
//{
//    return (this->_headers);
//}
//
//ByteVector Request::get_body_text()
//{
//    throw std::runtime_error("This method will be removed So you may use get_body()");
//}
//
///*
//void read_rest()
//{
//
//
//}
//*/
//
///*
//bool Request::read_header()
//{
//    if (_header_read_complete){
//        return ;
//    }
//}
//*/
//
//ByteVector Request::read_body()
//{
//    if (is_full_body_loaded())
//        return ByteVector();
//    ByteVector bytes;
//    ByteVector tmp = _buf.get_extra_buf();
//    if (tmp.size() > 0)
//        bytes = tmp;
//    else
//        bytes = _buf.get_body(BUF_MAX);
//
//    _body.insert(_body.end(), bytes.begin(), bytes.end());
//    return bytes;
//}
//
//string const& Request::get_path() const
//{
//    return _path;
//}
//
//// path/to/file -> path, to, file
//vector<string> Request::get_path_list() const
//{
//    vector<string> path_list;
//    string buf;
//    size_t i = 0;
//    while (_path[i] != 0) {
//        if ((_path[i] == '/' || i == _path.length() - 1) && buf != "") {
//            path_list.push_back(buf);
//            buf = "";
//        } else
//            buf += _path[i];
//        i++;
//    }
//    return (path_list);
//}
//
//const URI& Request::get_uri()
//{
//    return (this->_uri);
//}
//
//string Request::get_ip_address() const
//{
//    // todo
//    return ("127.0.0.1");
//}
//
//string Request::get_domain() const
//{
//    // todo
//    return ("test.com");
//}
//
//ssize_t Request::get_content_length() const
//{
//    return (this->_content_length);
//}
//
//string Request::get_transfer_encoding() const
//{
//    return (this->_transfer_encoding.get_str());
//}
//
//ByteVector Request::get_body() const
//{
//    if (!is_full_body_loaded())
//        throw std::runtime_error("Request::get_body() body is not loaded");
//    return _body;
//}
//
//vector<ByteVector> Request::get_body_splitted() const
//{
//    if (!is_full_body_loaded())
//        throw std::runtime_error("Request::get_body_splitted() body is not loaded");
//    if (!_content_type.is_multipart())
//        throw std::runtime_error("Request::get_body_splitted() can only use  multipart/*. This content-type is " +
//                                 _content_type.get_media_type());
//
//    string boundary = _content_type.get_boundary();
//    vector<ByteVector> body_list;
//    string body = _body.get_str();
//
//    if (body.find(boundary) == string::npos) {
//        throw std::runtime_error("Request::get_body_splitted() boundary is not found");
//    }
//
//    Split ss(body, boundary + "\r\n");
//    vector<string>::iterator its = ss.begin();
//    vector<string>::iterator ite = ss.end();
//    while (its != ite) {
//        body_list.push_back(ByteVector((*its).c_str(), (*its).length()));
//        its++;
//    }
//    return body_list;
//}
//
//bool Request::is_cgi() const
//{
//    return (_uri.is_cgi());
//}
//
//Server const* Request::get_server_config() const
//{
//    try {
//        return Config::get_instance()->http->get_server(_headers.at("Host"));
//    } catch (const std::exception& e) {
//        return Config::get_instance()->http->server.at(0);
//    }
//}
//
//bool is_prefix(const std::string& strA, const std::string& strB)
//{
//    if (strB.size() > strA.size()) {
//        return false;
//    }
//    return strA.find(strB) == 0;
//}
//
///*
//Config const* Request::get_config() const
//{
//    return _config;
//}
//*/
//
//bool Request::is_full_body_loaded() const
//{
//    if (_method == HttpMethod::GET)
//        return true; // ここは例外を発生させたほうが誤用が少なくなってよいかも？
//    if (_transfer_encoding == TransferEncoding::CHUNKED)
//        return _body.end_with("0\r\n\r\n");
//    return _body.size() >= static_cast<unsigned long>(_content_length);
//}
//
//std::string& Request::getport()
//{
//    return port;
//}
//
//std::string& Request::get_host()
//{
//    return (this->_headers["host"]);
//}
//
//ContentType const& Request::get_content_type() const
//{
//    return _content_type;
//}
//
//size_t Request::get_header_sum_size()
//{
//    return Utility::get_map_str_size(this->_headers);
//}
