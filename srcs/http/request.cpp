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
#include "http_exception.hpp"
#include "utility.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::tolower;
using std::vector;

#define BODY_TMP_DIRECTORY_PATH "/tmp/webserv_body_tmp/"

Request::Request() :
    file(NULL),
    //raw_buf_pos_(0),
    buf_body_size(0),
    // -1 is for '\0'
    //raw_buf_rest_size_(MAX_BUF-1),
    is_file_(false),
    is_directory_(false),
    is_not_executable_parent_dir_(false)
    //source_file(NULL)
    //is_redable_darectory(false)
{
    DEBUG("Request::Request()");
}

Request::~Request()
{
    //cout << "~Request()" << endl;
}


Request *Request::from_fd(FileDiscriptor fd)
{
    Request *req = new Request();
    req->fd_ = fd;
    //req->sockfd_ = sockfd;
    return (req);
}

FileDiscriptor Request::fd() const
{
    return (this->fd_);
}

/*
FileDiscriptor Request::sockfd() const
{
    return (this->sockfd_);
}
*/


/*
int Request::load_buf(FileDiscriptor fd, IReader *ireader)
{
;
}

void Request::insert_buf(char *buf, int size)
{
    //int max = MAX_BUF;
    int rest = MAX_BUF - (raw_buf_size + size);
    if(rest < 0){
        //WARNING("Request");

    }
    Utility::memcpy(this->raw_buf, buf, size);
    raw_buf
}
char *Request::buf()
{
    return (this->raw_buf);
}
*/

/*
void Request::decrement_raw_buf_size(size_t size)
{
    this->raw_buf_point -= size;
}

void Request::clear_raw_buf()
{
    this->raw_buf_pos_ = 0;
}
*/


char *Request::get_buf_body(int *size)
{
    *size = this->buf_body_size;
    DEBUG("Request::get_buf_body size:" + Utility::to_string(this->buf_body_size));
    return (this->buf_body);
}

void Request::set_buf_body(char *body_p, int size)
{
    DEBUG("Request::set_buf_body size:" + Utility::to_string(size));
    this->buf_body = body_p;
    this->buf_body_size = size;
}

void Request::clear_buf_body()
{
    this->buf_body_size = 0;
    this->buf_body = NULL;
}

/*
char *Request::get_raw_buf_begin()
{
    //DEBUG("Request::get_raw_buf_pointer()  raw_buf_point:" + Utility::to_string(this->raw_buf_pos_));
    return &(this->raw_buf[0]);
}
*/


/*
*/

std::string const &Request::requested_filepath() const
{
    return (this->requested_filepath_);
}

std::string const &Request::requested_path() const
{
    return (this->requested_path_);
}

std::string const &Request::tmp_path_info() const
{
    return (this->tmp_path_info_);
}

std::string const &Request::parent_dir_path() const
{
    return (this->parent_dir_path_);
}


/*
File Request::get_target_file(const ConfigLocation *location)
{

    //return &(this->raw_buf[this->raw_buf_point]);
}

int Request::raw_buf_space()
{
    DEBUG("Request::raw_buf_space()  raw_buf_pos_:" + Utility::to_string(this->raw_buf_pos_));
    return (MAX_BUF - 1 - this->raw_buf_pos_);
}

void Request::set_buf_pos(size_t pos)
{
    DEBUG("Request::set_buf_pos():" + Utility::to_string(pos));
    this->raw_buf_pos_ = pos;
    //DEBUG("Request::raw_buf_space()  raw_buf_rest_size_:" + Utility::to_string(this->raw_buf_rest_size_));
    //return (raw_buf_pos_);
}

*/
/*
void Request::renew_raw_buf_space(int space)
{
    this->raw_buf_rest_size_ = space;
    this->raw_buf_point = MAX_BUF-space;
}
*/

void Request::set_request_line(std::string const &str)
{
    this->req_line_ = RequestLine::from_string(str);
}

void Request::set_header(Split &sp, size_t offset)
{
    this->header_ = Header::from_splited_data(sp, offset);
}

void Request::set_read_completed(bool flag)
{
    this->read_completed_ = flag;
}

bool Request::read_completed()
{
    return (this->read_completed_);
}

void Request::check_parent_permittion(std::string &path)
{
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
    cout << "root:" << root << endl;
    cout << "uri_path:" << uri_path << endl;
    this->requested_path_ = path;
    this->tmp_path_info_ = "";
    MYINFO("Request::set_requested_filepath() requested_path=" + this->requested_path_);

    bool first = true;
    for(int i=uri_sp.size()-1; i >= 0;i--){
        if (Utility::is_regular_file(path)){
            size_t k = i+1;
            while (k <= uri_sp.size()-1){
                this->tmp_path_info_ += uri_sp[k] + "/";
                k++;
            }
            cout << "tmp path info:" << this->tmp_path_info_ << endl;
            cout << "tmp path info:" << this->tmp_path_info_ << endl;
            cout << "tmp path info:" << this->tmp_path_info_ << endl;
            cout << "tmp path info:" << this->tmp_path_info_ << endl;
            //requested_uri.set_path_info(tmp_path_info);

            this->requested_filepath_ = path;
            this->is_file_ = true;
            //this->is_redable_darectory = true;
            //this->is_redable_darectory = true;
            MYINFO("Request::set_requested_filepath filepath=" + path);
            return;
        }
        size_t pos = path.rfind(uri_sp[i]);
        if(pos == string::npos){
            //error
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
    //this->requested_directory_path = this->requested_path_;
}

void Request::set_is_file(bool flag)
{
    this->is_file_ = flag;
}

bool Request::is_file() const
{
    return (this->is_file_);
}

void Request::set_is_directory(bool flag)
{
    this->is_directory_ = flag;
}

void Request::set_cgi(bool flag)
{
    this->is_cgi_ = flag;
}

bool Request::is_cgi()
{
    return (is_cgi_);
}

bool Request::is_directory() const
{
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


/*
void Request::set_source_file(File *file)
{
    this->source = file;
}
*/

/*
File *Request::get_source_file()
{
    return (this->source);
}

int Request::open_source_file()
{
    return (this->source->open());
}

int Request::close_source_file()
{
    return (this->source->close());
}

ssize_t Request::get_data(char** data)
{
    return (this->source->read(data, MAX_READ_SIZE));
}
*/

int Request::open()
{
    DEBUG("Request::open_file()");
    if (this->file){
        DEBUG("Request::open_file() No.2");
        return (this->file->open());
    }
    DEBUG("Request::open_file() No.3");
    return 0;
}

int Request::close()
{
    if (this->file){
        return (this->file->close());
    }
    return 0;
}

int Request::write(char **data, size_t size)
{
    (void)data;
    (void)size;
    return (0);
}

int Request::save(char *buf, size_t size)
{
    for(size_t i=0;i<size;i++){
        this->tmp_buf.push_back(buf[i]);
    }
    return (this->tmp_buf.size());
}

bool Request::can_read()
{
    return (true);
}

bool Request::is_chunk()
{
    return (true);
}

size_t Request::size()
{
    return (0);
}

int Request::remove()
{
    return (0);
}

/*
std::string const &Request::path()
{
    std::runtime_error("Don't use");
    //std::string tmp = "";
    return (requested_filepath_);
}
*/

int Request::read(char** data, size_t max_read_size)
{
    return (this->file->read(data, max_read_size));
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
    //cout << " uri_raw: " << this->req_line.uri().raw() << endl;
    //cout << " version: " << this->req_line.version().to_string() << endl;

    cout << "|--------------------------|" << endl;
}

