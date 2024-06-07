#include "response.hpp"

#include <stdexcept>
#include <fstream>
#include <sys/stat.h>
#include <utility.hpp>
#include <fcntl.h>
#include "config.hpp"
#include "error_file.hpp"
#include "header_word.hpp"
#include "webserv_file_factory.hpp"

Response::Response() :
    buf_body_size(0),
    file(NULL),
    is_redirect(false),
    send_state(STILL_NOT_SEND),
    has_body_(false),
    is_chunked(false),
    written_body_size(0),
    header_index(0)
{
    DEBUG("Response Constructor");
    this->headers.insert("Server", WEBSERV_VERSION);
    this->headers.insert("Date", Utility::time_to_string());
}

Response::~Response()
{
    DEBUG("Response Destructor");
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
    this->file = res.file;
    this->is_redirect = res.is_redirect;
    this->send_state = res.send_state;
    this->status_line = res.status_line;
    return (*this);
}

Response *Response::change_error_code(Response* res, FileDiscriptor const &fd, StatusCode &code, ConfigLocation const *location)
{
    DEBUG("Response::change_error_code:" + code.to_string());
    (void)fd;
    std::map<StatusCode, std::string> const errors = location->error_pages();
    std::map<StatusCode, std::string>::const_iterator error_ite = errors.find(code);
    if(error_ite != errors.end()){
        if(location->error_replaced_code().to_int() != 0){
            res->status_code = location->error_replaced_code();
        }else{
            res->status_code = code;
        }
        WebservFileFactory *file_factory = WebservFileFactory::get_instance();
        (void)file_factory;
        res->file = file_factory->make_normal_file(fd, error_ite->second, std::ios::in);
        res->add_header(CONTENT_LENGTH, Utility::to_string(res->file->size()));
        res->has_body_ = true;
    }

    return (res);
}

Response* Response::from_success_status_code(FileDiscriptor const &fd, StatusCode &code, WebservFile *file, ConfigLocation const *location)
{
    (void)location;
    DEBUG("Response::from_success_status_code");
    Response *res = new Response();

    std::map<StatusCode, std::string> const errors = location->error_pages();
    std::map<StatusCode, std::string>::const_iterator error_ite = errors.find(code);

    if(error_ite != errors.end()){
        res = res->change_error_code(res, fd, code, location);
    }else{
        res->status_code = code;
        res->file = file;
    }

    return (res);
}

Response* Response::from_error_status_code(FileDiscriptor const &fd, StatusCode &code, ConfigLocation const *location)
{
    DEBUG("Response::from_error_status_code fd:" + fd.to_string());
    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    Response *res = new Response();

    if(location){
        std::map<StatusCode, std::string> const errors = location->error_pages();
        std::map<StatusCode, std::string>::const_iterator error_ite = errors.find(code);

        if(error_ite != errors.end() && Utility::is_readable_file(error_ite->second)){
            res = res->change_error_code(res, fd, code, location);
            return (res);
        }
    }
    res->status_code = code;
    res->file = file_factory->make_error_file(fd, code);
    res->add_header(CONTENT_LENGTH, Utility::to_string(res->file->size()));
    res->has_body_ = true;

    return (res);
}

void Response::set_header(Split &sp, size_t offset)
{
    this->headers = Header::from_splited_data(sp, offset);
}

Response* Response::from_cgi_header_line(FileDiscriptor const &fd, Split &header_line, WebservFile *file)
{
    DEBUG("from_cgi_header_line");
    Response *res = new Response();
    res->file = file;
    try{
        res->set_header(header_line, 0);
        std::string status_code = res->headers.find(STATUS_CODE_CGI);

        if(status_code == res->headers.not_find()){
            res->status_code = StatusCode::from_int(200);
        }else{
            Split tmp(status_code, " ");
            res->status_code = StatusCode::from_string(tmp[0]);
            res->headers.erase(STATUS_CODE_CGI);
        }
    }catch(std::invalid_argument &e){
        ERROR("Making Response ERROR: header line is invalid " );
        delete res;
        throw HttpException("500");
    }

    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    WebservFile *dummy = file_factory->make_dummy_file(fd, res);
    (void)dummy;

    return (res);
}

const Header &Response::header()
{
    return (this->headers);

}

bool Response::check_body_size(ConfigServer const *server)
{
    DEBUG("Response::check_body_size");
    string const &len_str = this->header().find(CONTENT_LENGTH);

    if(len_str != this->header().not_find()){
        size_t len = Utility::to_size_t(len_str);
        if(len >= server->get_max_body_size()){
            ERROR("Making Response ERROR: body size is exceed:" + Utility::to_string(len));
            return (false);
        }
    }else if(this->is_chunk()){
        this->has_body_ = true;
    }
    return (true);
}

char* Response::get_buf_body(int *size)
{
    *size = this->buf_body_size;
    return &(this->buf_body[0]);
}

void Response::set_buf_body(char *body_p, int size)
{
    DEBUG("Response::set_buf_body size:" + Utility::to_string(size));
    this->buf_body_size = size;
    this->buf_body.resize(size);
    if(size > 0){
        this->has_body_ = true;
        for(int i=0;i<size;i++){
            this->buf_body[i] = body_p[i];
        }
    }
}

void Response::add_header(std::string const &key, std::string const &value)
{
    this->headers.insert(key, value);
}


void Response::make_status_line()
{
    this->status_line = ("HTTP/1.1 " +  status_code.to_string() + " " + status_code.message() + "\r\n");
}

void Response::make_header_line()
{
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
    DEBUG("Response::read_body_and_copy");
    if(this->buf_body_size > 0){
        for(int i=0;i<this->buf_body_size;i++){
            (*dst)[i] = (this->buf_body[i]);
        }
        int tmp = this->buf_body_size;
        this->buf_body_size = 0;
        return (tmp);
    }
    if(file){
        return (this->file->read(dst, size));
    }
    return (-1);
}

int Response::read_body_and_copy_chunk(char** dst, size_t size)
{
    const int  MAX_CHUNKED_SIZE = 20;
    char *dst_p = *dst;
    int read_size = 0;

    if(this->buf_body_size > 0){

        read_size = this->buf_body_size;
        Utility::memcpy(&(dst_p[MAX_CHUNKED_SIZE]), &(this->buf_body[0]), this->buf_body_size);
        dst_p[MAX_CHUNKED_SIZE + this->buf_body_size] = '\0';
        this->buf_body_size = 0;
    }else{
        char *tmp = &(dst_p[MAX_CHUNKED_SIZE]);
        read_size = this->file->read(&tmp, size - MAX_CHUNKED_SIZE);
    }

    // chunkサイズは16進数
    std::string size_str = Utility::to_hexstr(read_size);
    size_str += CRLF;
    size_t len = size_str.size();
    Utility::memcpy(dst_p, size_str.c_str(), len);
    Utility::memcpy(&(dst_p[len]), &(dst_p[MAX_CHUNKED_SIZE]), read_size);

    if (read_size > 0){
        Utility::memcpy(&(dst_p[len+read_size]), "\r\n", 2);
        read_size += 2;
    }else{
        Utility::memcpy(dst_p, "0\r\n\r\n", 5);
        return 5;
    }

    return (read_size+len);
}

int Response::open()
{
    DEBUG("Response::open_file()");
    if (this->file){
        return (this->file->open());
    }
    return 0;
}

int Response::close()
{
    return 0;
}

int Response::write(char **data, size_t size)
{
    (void)data;
    (void)size;
    return (0);
}

bool Response::is_chunk()
{
    DEBUG("Response::is_chunk():" + Utility::to_string(this->is_chunked));
    return (this->is_chunked);
}

bool Response::has_body()
{
    DEBUG("Response::has_body();" + Utility::to_string(this->has_body_));
    return (this->has_body_);
}

bool Response::check_body_and_chunk()
{
    this->has_body_ = false;
    this->is_chunked = false;

    std::string const &enc = this->headers.find(TRANSFER_ENCODING);
    if(enc != this->headers.not_find()){
        if(enc == TRANSFER_ENCODING_CHUNKED){
            this->is_chunked = true;
            this->has_body_ = true;
        }
    }
    std::string const &len = this->headers.find(CONTENT_LENGTH);
    if(len != this->headers.not_find()){
        if(Utility::to_int(len) > 0){
            this->has_body_ = true;
        }
    }

    if(this->is_chunked == false && this->has_body_ == false){
        return (false);
    }
    return (true);

}

bool Response::read_completed(){
    return (this->send_state == SENT_BODY);
}


int Response::read(char** data, size_t max_read_size)
{
    char *tmp;

    bool ref = true;
    int read_size = read_data(&tmp, data,  max_read_size , ref);
    if(read_size <= 0){
        return read_size;
    }

    if(ref){
        for(int i=0; i<read_size; i++){
            (*data)[i] = tmp[i];
        }
    }
    return (read_size);
}

int Response::read_data(char** ref, char **cp, size_t max_read_size, bool &ref_flag)
{
    (void)max_read_size;
    if(this->send_state == SENT_BODY){
        DEBUG("Response::read: SENT_BODY");
        return -1;
    }else if (this->send_state == STILL_NOT_SEND) {
        DEBUG("Response::read: STILL_NOT_SEND");
        this->make_status_line();
        *ref = const_cast<char*>(&(this->status_line[0]));
        this->send_state = SENT_STATUS_LINE;

        MYINFO("\nResponse Status Line:" + this->status_line);
        this->make_header_line();
        return (this->status_line.size());
    }else if (this->send_state == SENT_STATUS_LINE){
        DEBUG("Response::read: SENT_STATUS_LINE");
        *ref = const_cast<char*>(&(this->header_line[this->header_index]));

        size_t return_size = this->header_line.size() - this->header_index;
        if(max_read_size >= return_size){
            this->send_state = SENT_HEADER;
            this->header_index = this->header_line.size();
        }else{
            return_size = max_read_size;
            this->header_index += max_read_size;
        }
        return (return_size);
    }else if (this->send_state == SENT_HEADER){
        ref_flag = false;
        if(this->has_body()){
            int size=0;
            if (this->is_chunked){
                size = this->read_body_and_copy(cp, max_read_size);
                //size = this->read_body_and_copy_chunk(cp, max_read_size);
                if (size == 5 && Utility::memcpy(*cp, "0\r\n\r\n", 5)){
                    this->send_state = SENT_BODY;
                }
            }else{
                ssize_t content_length = this->headers.get_content_length();
                size = this->read_body_and_copy(cp, max_read_size);
                if(size > 0){
                    this->add_written_body_size((size_t)size);
                }

                if(this->written_body_size >= (size_t)content_length){
                    DEBUG("Response:: change to SENT_BODY ");
                    this->send_state = SENT_BODY;
                }
            }
            return (size);
        }else{
            ref_flag = false;
            this->send_state = SENT_BODY;
            (*cp)[0] = '\r';
            (*cp)[1] = '\n';
            return (0);
        }
    }
    return (0);
}


WebservFile *Response::get_file()
{
    return (this->file);

}


void Response::print_info()
{
    cout << "|-- Print Response --|" << endl;
    cout << "status code:" << this->status_code.to_string() << endl;
    cout << "|--------------------------|" << endl;
}

void Response::add_written_body_size(size_t size)
{
    this->written_body_size += size;
}

void Response::switching_file(WebservFile *file)
{
    this->file = file;
}

std::vector<char> &Response::buffer()
{
    return (this->buf_body);
}

void Response::clear_buffer()
{
    this->buf_body.clear();
    buf_body_size = 0;
}
