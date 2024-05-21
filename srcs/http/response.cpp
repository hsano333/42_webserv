#include "response.hpp"

#include <stdexcept>
#include <fstream>
#include <sys/stat.h>
#include <utility.hpp>
#include <fcntl.h>
#include "config.hpp"
#include "error_file.hpp"
#include "header_word.hpp"

Response::Response() :
    //buf_body(NULL),
    buf_body_size(0),
    file(NULL),
    is_redirect(false),
    send_state(STILL_NOT_SEND),
    has_body(false),
    is_chunked(false),
    written_body_size(0)
    //exist_body_(false)
{
    DEBUG("Response Constructor");
    this->headers.insert("Server", WEBSERV_VERSION);
    this->headers.insert("Date", Utility::time_to_string());
}

Response::~Response()
{
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
    //this->exist_body_ = res.exist_body_;
    return (*this);
}

Response* Response::from_success_status_code(StatusCode &code, WebservFile *file)
{
    DEBUG("Response::from_success_status_code");
    Response *res = new Response();
    res->status_code = code;
    res->file = file;
    //res->exist_body_ = false;
    return (res);
}

/*
Response* Response::from_error_status_code(StatusCode &code)
{
    DEBUG("Response::from_error_status_code");
    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    Response *res = new Response();
    res->status_code = code;
    //res->file = ErrorFile::from_status_code(code);
    res->file = file_factory->make_error_file( , code);
    //res->exist_body_ = true;
    return (res);
}
*/

Response* Response::from_error_file(WebservFile *file, StatusCode &code)
{
    DEBUG("Response::from_error_file");
    Response *res = new Response();
    res->status_code = code;
    res->file = file;
    //res->exist_body_ = true;
    return (res);
}

Response* Response::from_file(WebservFile *file)
{
    DEBUG("Response::from_file");
    Response *res = new Response();
    res->file = file;
    if (file->can_read()){
        res->status_code = StatusCode::from_int(200);
    }else{
        res->status_code = StatusCode::from_int(406);
    }
    return (res);
}


void Response::set_header(Split &sp, size_t offset)
{
    this->headers = Header::from_splited_data(sp, offset);
}

//Response* Response::from_cgi_header_line(Split &header_line, File *file, ConfigServer const *server, ConfigLocation const *location)
Response* Response::from_cgi_header_line(Split &header_line, WebservFile *file)
{
    //(void)location;
    Response *res = new Response();
    res->file = file;
    res->set_header(header_line, 0);
    std::string status_code = res->headers.find(STATUS_CODE_CGI);
    try{
        if(status_code == res->headers.not_find()){
            res->status_code = StatusCode::from_int(200);
        }else{
            Split tmp(status_code, " ");
            res->status_code = StatusCode::from_string(tmp[0]);
            res->headers.erase(STATUS_CODE_CGI);
        }
    }catch(std::invalid_argument &e){
        ERROR("Making Response ERROR: Status Code is invalid " + status_code);
        delete res;
        throw HttpException("500");
    }
    /*
    */

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
    DEBUG("Response::check_body_size No.1");

    if(len_str != this->header().not_find()){
        size_t len = Utility::to_size_t(len_str);
    DEBUG("Response::check_body_size No.2");
        if(len >= server->get_max_body_size()){
            ERROR("Making Response ERROR: body size is exceed:" + Utility::to_string(len));
            //delete res;
            //throw HttpException("500");
            return (false);
        }
    }else if(this->is_chunk()){
    DEBUG("Response::check_body_size No.3");
        this->has_body = true;
    }
    DEBUG("Response::check_body_size No.4");
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
    //this->buf_body = body_p;
    this->buf_body_size = size;
    this->buf_body.resize(size);
    if(size > 0){
        this->has_body = true;
        for(int i=0;i<size;i++){
            this->buf_body[i] = body_p[i];
        }
    }
}


/*
void Response::set_exist_body(bool flag)
{
    this->exist_body_ = flag;
}
*/

void Response::add_header(std::string const &key, std::string const &value)
{
    this->headers.insert(key, value);
}


/*
File *Response::get_source_file()
{
    return (this->file);
}
*/


void Response::make_status_line()
{
    this->status_line = ("HTTP/1.1 " +  status_code.to_string() + " " + status_code.message() + "\r\n");
}

void Response::make_header_line()
{
    //this->header_line = ("HTTP/1.1 " +  status_code.to_string() + " " + status_code.message());
    //if (this->exist_body_ ){
        /*
        if (this->file->is_chunk()){
            this->header_line = "Transfer-Encoding: chunked\r\n";
        }else{
            this->header_line = "Content-Length: " + Utility::to_string(this->file->size()) + "\r\n";
        }
        */
    //}else{
        //this->header_line = "Content-Length: 0\r\n";
    //}
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
        DEBUG("Response::read_body_and_copy buf_body_size:" + Utility::to_string(this->buf_body_size));
        cout << "Response body.size()= " << this->buf_body.size() << endl;
        cout << "Response body_p[i] Test:[" ;
        for(int i=0;i<this->buf_body_size;i++){
            printf("%c", this->buf_body[i]);

            (*dst)[i] = (this->buf_body[i]);
            //cout << this->buf_body[i];
        }
    cout << "]" << endl;
        int tmp = this->buf_body_size;
        this->buf_body_size = 0;
        return (tmp);
    }
    DEBUG("read_body_and_copy:" + Utility::to_string(size));
    if(file){
        DEBUG("file is exist");
    }else{
        DEBUG("file is not exist:NULL");
    }
    return (this->file->read(dst, size));
}

int Response::read_body_and_copy_chunk(char** dst, size_t size)
{
    DEBUG("Response::read_body_and_copy_chunk");
    //printf("tmp=%p\n", tmp);
    const int  MAX_CHUNKED_SIZE = 20;
    char *dst_p = *dst;
    int read_size = 0;
    DEBUG("Response::read chunked No.4:");

    if(this->buf_body_size > 0){

        read_size = this->buf_body_size;
        Utility::memcpy(&(dst_p[MAX_CHUNKED_SIZE]), &(this->buf_body[0]), this->buf_body_size);
        //std::string size_str = Utility::to_hexstr(this->buf_body_size);
        dst_p[MAX_CHUNKED_SIZE + this->buf_body_size] = '\0';
        DEBUG("chunk buffer =" + Utility::to_string(dst_p));
        DEBUG("buffer_size=" + Utility::to_string(this->buf_body_size));
        //for(int i=0;i<this->buf_body_size;i++){
            //(*dst)[i] = (this->buf_body[i]);
            ////cout << this->buf_body[i];
        //}
    //cout << "]" << endl;
        //int tmp = this->buf_body_size;
        this->buf_body_size = 0;
        //return (tmp);
    }else{

        DEBUG("Response::read chunked No.4-2 size=:" + Utility::to_string(size));
        char *tmp = &(dst_p[MAX_CHUNKED_SIZE]);
        read_size = this->file->read(&tmp, size - MAX_CHUNKED_SIZE);

    }
    DEBUG("Response::read chunked No.5:");
    //int read_size = this->file->read(&(tmp[chunk_size]), size - chunk_size);

    // chunkサイズは16進数
    //string size_str = CRLF;
    std::string size_str = Utility::to_hexstr(read_size);
    DEBUG("Response::read chunked No.6:" + size_str);
    size_str += CRLF;
    size_t len = size_str.size();
        DEBUG("Response::read chunked No.7:");
    Utility::memcpy(dst_p, size_str.c_str(), len);
    Utility::memcpy(&(dst_p[len]), &(dst_p[MAX_CHUNKED_SIZE]), read_size);

    DEBUG("Response::read chunked No.8:");
    if (read_size > 0){
        Utility::memcpy(&(dst_p[len+read_size]), "\r\n", 2);
        read_size += 2;
        //転送終了時だけ改行を付けない
        //(*dst)[read_size] = '\r';
        //tmp2[read_size+1] = '\n';
        //read_size += 2;
    }else{
        Utility::memcpy(dst_p, "0\r\n\r\n", 5);
        return 5;
    }

    return (read_size+len);
}


/*
int Response::read_body_and_copy_chunk(char** dst, size_t size)
{
    DEBUG("Response::read_body_and_copy_chunk");
    char *tmp = *dst;
    printf("tmp=%p\n", tmp);
    int chunk_size = 20;
    DEBUG("Response::read chunked No.4:");
    char *tmp2 = &(tmp[chunk_size]);

    DEBUG("Response::read chunked No.4-2 size=:" + Utility::to_string(size));
    printf("tmp2=%p\n", tmp2);
    //int read_size = 10;
    int read_size = this->file->read(&(tmp2), size - chunk_size);
    DEBUG("Response::read chunked No.5:");
    //int read_size = this->file->read(&(tmp[chunk_size]), size - chunk_size);

    // chunkサイズは16進数
    //string size_str = CRLF;
    std::string size_str = Utility::to_hexstr(read_size);
        DEBUG("Response::read chunked No.6:");
    size_str += CRLF;
    size_t len = size_str.size();
        DEBUG("Response::read chunked No.7:");
    Utility::memcpy(&(tmp[chunk_size-len]), size_str.c_str(), len);
        DEBUG("Response::read chunked No.8:");
    *dst = &(tmp[chunk_size-len]);
        DEBUG("Response::read chunked No.9:");
    *dst = &(tmp[chunk_size-len]);
        DEBUG("Response::read chunked No.10:");
    //if (read_size > 0){
        //転送終了痔だけ改行を付けない
        tmp2[read_size] = '\r';
        tmp2[read_size+1] = '\n';
        read_size += 2;
    //}

    return (read_size+len);
}
*/

int Response::open()
{
    DEBUG("Response::open_file()");
    if (this->file){
        DEBUG("Response::open_file() No.2");
        return (this->file->open());
    }
    DEBUG("Response::open_file() No.3");
    return 0;
}

int Response::close()
{
    //if (this->file){
        //return (this->file->close());
    //}
    return 0;
}

int Response::write(char **data, size_t size)
{
    (void)data;
    (void)size;
    return (0);
}

/*
int Response::save(char *buf, size_t size)
{
    for(size_t i=0;i<size;i++){
        this->tmp_buf.push_back(buf[i]);
    }
    return (this->tmp_buf.size());
}

bool Response::can_read()
{
    return (true);
}


size_t Response::size()
{
    return (0);
}

int Response::remove()
{
    return (0);
}

std::string const &Response::path()
{
    std::runtime_error("Don't use");
    //std::string tmp = "";
    return (status_line);
}
*/


bool Response::is_chunk()
{
    DEBUG("Response::is_chunk()");
    std::string enc = this->headers.find(TRANSFER_ENCODING);
    if(enc != this->headers.not_find()){
        if(enc == TRANSFER_ENCODING_CHUNKED){
            return (true);
        }
    }
    return (false);
}

bool Response::check_body_and_chunk()
{
    this->has_body = false;
    this->is_chunked = false;

    std::string const &enc = this->headers.find(TRANSFER_ENCODING);
    if(enc != this->headers.not_find()){
        if(enc == TRANSFER_ENCODING_CHUNKED){
            this->is_chunked = true;
            this->has_body = true;
        }
    }
    std::string const &len = this->headers.find(CONTENT_LENGTH);
    if(len != this->headers.not_find()){
        if(Utility::to_int(len) > 0){
            this->has_body = true;
        }
    }

    DEBUG("Response::check_body_and_chunk() chunked:" + Utility::to_string(this->is_chunked));
    if(this->is_chunked == false && this->has_body == false){
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
    DEBUG("Response::read()");

    bool ref = true;
    int read_size = read_data(&tmp, data,  max_read_size , ref);
    if(read_size <= 0){
        return read_size;
    }
    DEBUG("Response::read() No.2 read_size=" + Utility::to_string(read_size));
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
    DEBUG("Response::read:");
    //cout << "this->exist_body_:" << this->exist_body_ << endl;
    if(this->send_state == SENT_BODY){
        DEBUG("Response::read: SENT_BODY");
        return -1;
    }else if (this->send_state == STILL_NOT_SEND) {
        DEBUG("Response::read: STILL_NOT_SEND");
        this->make_status_line();
        *ref = const_cast<char*>(&(this->status_line[0]));
        this->send_state = SENT_STATUS_LINE;

        MYINFO("\nResponse Status Line:" + this->status_line);
        return (this->status_line.size());
    }else if (this->send_state == SENT_STATUS_LINE){
        DEBUG("Response::read: SENT_STATUS_LINE");
        this->make_header_line();
        *ref = const_cast<char*>(&(this->header_line[0]));
        MYINFO("Response  Header:" + string(*ref));
        this->send_state = SENT_HEADER;
        return (this->header_line.size());
    }else if (this->send_state == SENT_HEADER){
        ref_flag = false;
        if(this->has_body){
            DEBUG("Response::read: SENT_HEADER");
            int size=0;
            DEBUG("Response::read chunked No.1:");
            if (this->is_chunked){
                //size = this->read_body_and_copy(cp, max_read_size);
                size = this->read_body_and_copy_chunk(cp, max_read_size);
                DEBUG("Response::read chunked No.12 size:" + Utility::to_string(size));
                if (size == 5 && Utility::memcpy(*cp, "0\r\n\r\n", 5)){
                    DEBUG("Response::read chunked No.13:");
                    this->send_state = SENT_BODY;
                }
            }else{
                ssize_t content_length = this->headers.get_content_length();
                //cout << " not chunk" << endl;
            //DEBUG("Response::read chunked No.12:");
            //todo 
            // compare readed size and file size;
                //size_t file_size = this->file->size();
                size = this->read_body_and_copy(cp, max_read_size);
                if(size > 0){
                    this->add_written_body_size((size_t)size);
                }

                DEBUG("Response:: content_length:" + Utility::to_string(content_length));
                DEBUG("Response:: written_body_size:" + Utility::to_string(this->written_body_size));
                if(this->written_body_size >= (size_t)content_length){
                    DEBUG("Response:: change to SENT_BODY ");
                    this->send_state = SENT_BODY;
                }



                /*
                if (size <= 0){
            DEBUG("Response::read chunked No.13:");
                    this->send_state = SENT_BODY;
                }
                */

            }
            /*
            size = this->read_body_and_copy(cp, max_read_size);
            DEBUG("Response::read chunked No.12 size:" + Utility::to_string(size));
            if (size <= 0){
                DEBUG("Response::read chunked No.13:");
                this->send_state = SENT_BODY;
            }
            */

            DEBUG("Response::read chunked No.13:");
            return (size);
        }else{
            ref_flag = false;
            this->send_state = SENT_BODY;
            (*cp)[0] = '\r';
            (*cp)[1] = '\n';
            return (0);
        }
    }
    DEBUG("Response::read: 0");
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

