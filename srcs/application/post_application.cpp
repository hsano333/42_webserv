#include "post_application.hpp"
#include "http_exception.hpp"
#include "normal_reader.hpp"
#include "normal_file.hpp"
#include "directory_file.hpp"
#include "connection_exception.hpp"

PostApplication::PostApplication() : res(NULL)
{
;
}

PostApplication::~PostApplication()
{
;
}

File *PostApplication::get_requested_file()
{
    if (this->is_cgi_){
        return (NULL);
    }
    if (this->event->file()){
        return (this->event->file());
    }

    this->check_permission();

    //File *file = NULL;
        //this->req->print_info();
        if (this->req->is_not_executable_parent_dir()){
            ERROR("Parent directory is not x permission:" + this->req->parent_dir_path());
            throw HttpException("403");
        }
        else if(this->req->is_directory()){
            ERROR("directory already exist :" + this->req->parent_dir_path());
            throw HttpException("403");
        }else if (Utility::is_regular_file(this->req->requested_path())){
            ERROR("file already exist :" + this->req->requested_path());
            throw HttpException("403");
        }
        File *file = NormalFile::from_filepath(this->req->requested_path(), std::ios::out | std::ios::binary);
        this->event->set_file(file);
        return (file);
}


bool PostApplication::is_cgi() const
{
    return (this->is_cgi_);
}

bool PostApplication::check_not_cgi_end(size_t received_size)
{
    (void)received_size;

    return (true);
}

bool PostApplication::execute_not_cgi()
{
    DEBUG("PostApplication::execute_not_cgi");

    File *file = this->get_requested_file();
    MYINFO("PostApplication::execute_not_cgi:filez=" + file->path());


    Request *req = this->event->req();
    size_t max_len = req->header().get_content_length();
    MYINFO("PostApplication::execute_not_cgi:max_len=" + Utility::to_string(max_len));

    //char **tmp;
    int size = 0;
    char *body_buf = req->get_buf_body(&size);
    MYINFO("PostApplication::execute_not_cgi:size=" + Utility::to_string(size));
    MYINFO("PostApplication::No.1");

    try{
        int open_result = file->open();
        MYINFO("PostApplication::No.3 open:" + Utility::to_string(open_result));

        // openに成功することは、はじめてここにきてことを意味する。
        // よって、まだ書き込まれていないbodyデータを書き込む必要がある。
        if(size > 0)
        {
            int result = file->write(&body_buf, size);
            MYINFO("PostApplication::No.2 result=" + Utility::to_string(result));
            if(result < 0){
                throw (HttpException("500"));
            }
            req->clear_buf_body();
        }
    }catch(std::runtime_error &e){
        WARNING("catch open error. but ignore");
    }

    char* buf = req->get_raw_buf_begin();
    //req->clear_raw_buf();
    FileDiscriptor fd = event->fd();

    //int space = req->raw_buf_space();
    //size_t sum = req->buf_size();
    //MYINFO("PostApplication::read sum:" + Utility::to_string(sum));
    //size_t pos = 0;
    //(void)file;
    //int result = this->file->write(&body_buf, size);
    size_t sum = size;
    this->event->set_completed(false);
    while(this->event->is_completed() == false)
    {
        MYINFO("PostApplication::No.4 complted:" + Utility::to_string(req->read_completed()));
        //MYINFO("PostApplication::No.4 space:" + Utility::to_string(space));
        int tmp = this->reader->read(fd, &(buf[0]), MAX_READ_SIZE, NULL);
        MYINFO("PostApplication::No.5 read:" + Utility::to_string(tmp));
        if(tmp < 0){
            MYINFO("PostApplication::No.41");
            ERROR("Receiver Error:" + Utility::to_string(tmp));
            this->event->set_completed(true);
            break;
        }else if(tmp == 0){
            WARNING("Connection Error: read return is 0");
            throw ConnectionException("Read Error");
            break;
        }
        sum += tmp;
        if(sum >= max_len){
            MYINFO("PostApplication::No.42");
            MYINFO("PostApplication::read sum:" + Utility::to_string(sum));
            MYINFO("PostApplication::max_len:" + Utility::to_string(max_len));
            tmp = tmp - (sum - max_len);
            //req->set_read_completed(true);
        }
        MYINFO("PostApplication::No.7 sum:" + Utility::to_string(sum));
        int result = file->write(&buf, tmp);
        MYINFO("PostApplication::No.8 write :" + Utility::to_string(result));
        if(result < 0){
            throw std::exception(HttpException("500"));
        }else if(result == 0){
            WARNING("Connection Error: write return is 0");
            this->event->set_completed(true);
            break;
        }
    MYINFO("PostApplication::No.6:" + Utility::to_string(req->read_completed()));
    }
    MYINFO("PostApplication::No.7");

    MYINFO("PostApplication::write sum:" + Utility::to_string(sum));
    MYINFO("PostApplication::max_len:" + Utility::to_string(max_len));
    req->set_buf_body(NULL, sum);
    if (sum == max_len)
    {

        req->set_read_completed(true);
        file->close();
        MYINFO("PostApplication::No.8");
        delete file;
        MYINFO("PostApplication::No.9");
        //file = NULL;
        this->event->set_file(NULL);
        return (true);

    }else if(sum > max_len){
        ERROR("Too large body size:" + Utility::to_string(sum));
        throw std::exception(HttpException("400"));
    }
    return (false);
}


bool PostApplication::execute_cgi()
{
    return (true);

}

bool PostApplication::execute()
{
    if (this->is_cgi_){
        DEBUG("PostApplication::execute() CGI");
        return (this->execute_cgi());
    }else{
        DEBUG("PostApplication::execute() Not CGI");
        return (this->execute_not_cgi());
    }
    return (true);
}

void PostApplication::check_permission()
{
    const ConfigLimit *limit = this->location->limit();
    if (limit == NULL){
        WARNING("not permission: root:" + this->location->root());
        throw HttpException("403");
    }
    std::vector<Method> methods = limit->allowed_method();
    Method requested_method = this->req->req_line().method();
    for(size_t j = 0;j<methods.size();j++){
        if (requested_method == methods[j]){
            return ;
        }
    }

    // as like nginx, allowing Get method make HEAD method allowed.
    Method get_method = Method::from_string("GET");
    for(size_t j = 0;j<methods.size();j++){
        if (get_method == methods[j]){
            return ;
        }
    }
    WARNING("not permission: root:" + this->location->root());
    throw HttpException("403");
}

PostApplication* PostApplication::from_location(const Config *cfg, CGI *cgi, WebservApplicationEvent *event, IReader *reader)
{
    DEBUG("PostApplication::from_location");

    PostApplication *app = new PostApplication();
    app->cfg = cfg;
    app->req = event->req();
    app->server = cfg->get_server(app->req);
    app->location = cfg->get_location(app->server, app->req);
    app->cgi = cgi;
    app->path_info_ = app->req->tmp_path_info();
    //app->res = NULL;
    //app->is_continued = is_continued;
    //if(event->res()){
        //app->file = event->res()->get_file();
    //}
    app->event = event;
    app->reader = reader;

    try{
        app->cgi_application_path = string(cgi->get_cgi_application_path(app->req, app->location));
        app->is_cgi_ = true;
    }catch(std::invalid_argument &e){
        app->is_cgi_ = false;
    }

    return (app);
}

Response* PostApplication::make_response()
{
    StatusCode code = StatusCode::from_int(200);
    if (this->res){
        return (this->res);
    }
    return (Response::from_success_status_code(code, NULL));
    //return (Response::from_success_status_code(code));

    /*
    DEBUG("PostApplication::make_response()");
    File *file = this->get_requested_file();
    Response *res = NULL;

    if(this->location->is_redirect()){
        res = Response::from_file(file);
    }else if (this->req->is_file() || this->req->is_directory()){
        res = Response::from_file(file);
    }else if (this->req->is_directory()){
        res = Response::from_file(file);
    }else{
        ERROR("PostApplication::make_response(): Neither file nor directory");
        delete file;
        delete res;
        throw HttpException("403");
    }
    res->set_exist_body(true);

    std::map<std::string, std::string>::iterator ite = this->tmp_headers.begin();
    std::map<std::string, std::string>::iterator end = this->tmp_headers.end();
    while(ite != end){
        res->add_header(ite->first, ite->second);
        ite++;
    }
    return (res);
    */
}

/*
void PostApplication::set_path_info(std::string const &path_info)
{
    this->path_info_ = path_info;
}

std::string &PostApplication::get_path_info()
{
    return (this->path_info_);
}

std::string const &PostApplication::path_info() const
{
    return (this->path_info_);
}
*/

