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
#include "http_exception.hpp"
#include "utility.hpp"
#include "webserv_file_factory.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::tolower;
using std::vector;

#define BODY_TMP_DIRECTORY_PATH "/tmp/webserv_body_tmp/"


Request::Request(FileDiscriptor const &fd) :
    file_(NULL),
    fd_(fd),
    buf_body_size(0),
    buf_body_p(0),
    is_file_(false),
    is_directory_(false),
    is_not_executable_parent_dir_(false),
    is_deletable_parent_dir_(false),
    read_completed_(false),
    is_cgi_(false),
    read_body_size_(0),
    has_body_(false),
    auth_("")
{
    DEBUG("Request::Request()");
}

Request::~Request()
{
    DEBUG("Request: Destructor()");
}


Request *Request::from_fd(FileDiscriptor const &fd)
{
    DEBUG("Request::from_fd");
    Request *req = new Request(fd);

    (void)fd;
    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    WebservFile *dummy = file_factory->make_dummy_file(fd, req);
    (void)dummy;
    return (req);
}

FileDiscriptor Request::fd() const
{
    DEBUG("Request::fd");
    return (this->fd_);
}

void Request::add_buf_body_p(size_t size)
{
    DEBUG("Request::add_buf_body_p");
    this->buf_body_p += size;
}

char *Request::get_buf_body(size_t *size)
{
    DEBUG("Request::get_buf_body");
    *size = this->buf_body_size - this->buf_body_p;
    if(*size == 0){
        return NULL;
    }
    return &(this->buf_body[this->buf_body_p]);
}

void Request::set_buf_body(char *body, size_t size)
{
    DEBUG("Request::set_buf_body size:" + Utility::to_string(size));
    if(size <= 0){
        this->buf_body_size = 0;
        this->buf_body.clear();
        return ;
    }
    this->buf_body.resize(size+1);

    for(size_t i=0;i<size;i++){
        this->buf_body[i] = body[i];
    }
    this->buf_body[size] = '\0';
    this->buf_body_size = size;
}

void Request::clear_buf_body()
{
    DEBUG("Request::clear_buf_body");
    this->buf_body_size = 0;
    this->buf_body.clear();
}

std::string const &Request::requested_filepath() const
{
    DEBUG("Request::requested_filepath");
    return (this->requested_filepath_);
}

std::string const &Request::requested_path() const
{
    DEBUG("Request::requested_path");
    return (this->requested_path_);
}

std::string const &Request::tmp_path_info() const
{
    DEBUG("Request::tmp_path_info");
    return (this->tmp_path_info_);
}

std::string const &Request::parent_dir_path() const
{
    DEBUG("Request::parent_dir_path");
    return (this->parent_dir_path_);
}


void Request::set_request_line(std::string const &str)
{
    DEBUG("Request::set_request_line");
    this->req_line_ = RequestLine::from_string(str);
}

void Request::set_header(Split &sp, size_t offset)
{
    DEBUG("Request::set_header()");
    this->header_ = Header::from_splited_data(sp, offset);
}

void Request::set_read_completed(bool flag)
{
    DEBUG("Request::set_read_completed");
    this->read_completed_ = flag;
}

bool Request::read_completed()
{
    DEBUG("Request::read_completed");
    return (this->read_completed_);
}

void Request::check_parent_permittion(std::string &path)
{
    DEBUG("Request::check_parent_permittion");
    if (Utility::is_directory(path)){
        if (Utility::is_executable_directory(path)){
            this->is_not_executable_parent_dir_ = false;
        }else{
            this->is_not_executable_parent_dir_ = true;
        }
        if (Utility::is_deletable_directory(path)){
            this->is_deletable_parent_dir_ = true;
        }else{
            this->is_deletable_parent_dir_ = true;
        }
    }
}

void Request::set_requested_filepath(const ConfigLocation *location)
{
    DEBUG("Request::set_requested_filepath()");
    URI const &requested_uri = this->req_line().uri();
    const Split &uri_sp = requested_uri.splited_path();
    std::string const &uri_path = requested_uri.path();
    std::string const &root = location->root();

    std::string path = root + uri_path;
    this->requested_path_ = path;
    this->tmp_path_info_ = "";

    bool first = true;
    for(int i=uri_sp.size()-1; i >= 0;i--){
        if (Utility::is_regular_file(path)){
            size_t k = i+1;
            while (k <= uri_sp.size()-1){
                this->tmp_path_info_ += uri_sp[k] + "/";
                k++;
            }

            this->requested_filepath_ = path;
            this->is_file_ = true;
            return;
        }
        size_t pos = path.rfind(uri_sp[i]);
        if(pos == string::npos){
            return;
        }
        path = path.substr(0, pos-1);
        //tmp_path_info += uri_sp[i];
        // 対象ファイルのディレクトリが読み書き可能かどうか判定
        if (first){
            first = false;
            this->parent_dir_path_ = path;
            check_parent_permittion(path);
        }
    }

    this->is_file_ = false;
    if(Utility::is_directory(this->requested_path_)){
        this->is_directory_ = true;
    }
    this->requested_filepath_ = "";
}

void Request::set_is_file(bool flag)
{
    DEBUG("Request::set_is_file");
    this->is_file_ = flag;
}

bool Request::is_file() const
{
    DEBUG("Request::is_file");
    return (this->is_file_);
}

void Request::set_is_directory(bool flag)
{
    DEBUG("Request::set_is_directory");
    this->is_directory_ = flag;
}

void Request::set_cgi(bool flag)
{
    DEBUG("Request::set_cgi");
    this->is_cgi_ = flag;
}

bool Request::is_cgi()
{
    DEBUG("Request::is_cgi");
    return (is_cgi_);
}

bool Request::is_directory() const
{
    DEBUG("Request::is_directory");
    return (this->is_directory_);
}

bool Request::is_not_executable_parent_dir() const
{
    return (this->is_not_executable_parent_dir_);
}

bool Request::is_deletable_parent_dir() const
{
    return (this->is_deletable_parent_dir_);
}

RequestLine const &Request::req_line() const
{
    return (this->req_line_);
}

Header const &Request::header() const
{
    return (this->header_);
}

void Request::set_file(WebservFile *file)
{
    this->file_ = file;
}


void Request::set_auth(std::string const &str)
{
    this->auth_ = str;
}

std::string const &Request::auth()
{
    return (this->auth_);
}

// body以外のステータスライン、ヘッダーのサイズを返す
size_t Request::size()
{
    DEBUG("Request::size()");
    return (0);
}

int Request::read(char** data, size_t max_read_size)
{
    DEBUG("Request::read() max_size=" + Utility::to_string(max_read_size));
    int tmp = (this->file_->read(data, max_read_size));
    return (tmp);
}

std::string const &Request::path_info() const
{
    return (this->path_info_);
}

void Request::set_path_info(std::string const &root){
    this->path_info_ = root + tmp_path_info_;
}

void Request::add_read_body_size(size_t size)
{
    this->read_body_size_ += size;
}

bool Request::can_read()
{
    if(this->buf_body_size > 0){
        DEBUG("Request::can_read True");
        return (true);
    }
    DEBUG("Request::can_read False");
    return (false);
}

WebservFile *Request::file()
{
    DEBUG("Request::file()");
    return (this->file_);
}

void Request::set_has_body(bool flag)
{
    DEBUG("Request::set_has_body");
    this->has_body_ = flag;
}

bool Request::has_body()
{
    DEBUG("Request::has_body:" + Utility::to_string(this->has_body_));
    return (this->has_body_);
}


void Request::print_info() const
{
    MYINFO("|-- Print Request  --|");
    MYINFO("requested_filepath_ = " + this->requested_filepath_);
    MYINFO("requested_path_ = " + this->requested_path_);
    MYINFO("tmp_path_info_ = " + this->tmp_path_info_);
    MYINFO("|--------------------------|");


    cout << "|-- Print Request  --|" << endl;
    cout << "requested_filepath_ = " << this->requested_filepath_ << endl;
    cout << "requested_path_ = " << this->requested_path_ << endl;
    cout << "tmp_path_info_ = " << this->tmp_path_info_ << endl;
    this->req_line_.print_info();
    this->header_.print_info();

    cout << "|--------------------------|" << endl;
}

