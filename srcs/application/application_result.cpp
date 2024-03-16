#include "get_application.hpp"
#include "http_exception.hpp"
#include "normal_reader.hpp"
#include "normal_file.hpp"
#include "directory_file.hpp"
#include "application_result.hpp"
#include <unistd.h>

ApplicationResult::ApplicationResult() : app_(NULL), method_(Method::from_string("NONE")), is_cgi_(false)
{
    ;
}

ApplicationResult::ApplicationResult(StatusCode &code, Method const &method) : app_(NULL), code_(code), method_(method), is_cgi_(false)
{
    ;
}

ApplicationResult::ApplicationResult(StatusCode &code, string &method) : app_(NULL), code_(code), method_(Method::from_string(method)), is_cgi_(false)
{
    ;
}

ApplicationResult::~ApplicationResult()
{
;
}

int ApplicationResult::open()
{
    DEBUG("ApplicationResult::open()");
    if (this->file_){
        DEBUG("Request::open_file() No.2");
        return (this->file_->open());
    }
    return 0;
}

int ApplicationResult::read(char **buf, size_t max_size)
{
    DEBUG("ApplicationResult::read() size=" + Utility::to_string(max_size));
    (void)buf;
    (void)max_size;
    return (this->file_->read(buf, max_size));
    /*
    size_t size = this->buffer.retrieve(buf, max_size);
    if(size > 0){
        return (size);
    }

    if (this->state != FILE_OPEN){
        return (0);
    }
    return this->reader->read(this->fd, *buf, max_size, NULL);
    */
}

int ApplicationResult::write(char **buf, size_t size)
{
    DEBUG("ApplicationResult::write() size=" + Utility::to_string(size));
    (void)buf;
    (void)size;
    return (this->file_->write(buf, size));
    /*
    (void)size;
    if (this->state != FILE_OPEN){
        return (0);
    }
    return this->writer->write(this->fd, *buf, size, NULL);
    */
}

/*
int ApplicationResult::save(char *buf, size_t size)
{
    return (this->buffer.save(buf, size));
}

int ApplicationResult::save(char **buf, size_t size)
{
    (void)size;
    if (this->state != FILE_OPEN){
        return (0);
    }
    return this->reader->read(this->fd, *buf, size, NULL);
}
*/



int ApplicationResult::close()
{
    return (0);
}

/*
bool ApplicationResult::can_read()
{
    return (true);
}

size_t ApplicationResult::size()
{
    return (0);
    //return (this->text.size());
}

bool ApplicationResult::is_chunk()
{
    return (false);
}


int ApplicationResult::remove()
{
    // error file is not exist
    return (-1);
}
*/

/*
std::string const &ApplicationResult::path()
{
    return (this->text);
}
*/


Method const &ApplicationResult::method() const
{
    return (this->method_);
}

StatusCode const &ApplicationResult::status_code() const
{
    return (this->code_);
}

/*
void ApplicationResult::set_status_code(StatusCode &code)
{
    this->code_ = code;
}
*/

Header const &ApplicationResult::header() const
{
    return (this->header_);
}

void ApplicationResult::add_header(std::string const &key, std::string const &value)
{
    this->header_.insert(key, value);
}

void ApplicationResult::set_completed(bool flag)
{
    this->completed = flag;
}

ApplicationResult *ApplicationResult::from_fd(int in, int out, int pid)
{
    ApplicationResult *result = new ApplicationResult();
    result->cgi_in_ = FileDiscriptor::from_int(in);
    result->cgi_out_ = FileDiscriptor::from_int(out);
    result->pid_ = ProcessID::from_int(pid);
    return (result);
}

ApplicationResult *ApplicationResult::from_status_code(StatusCode &code, Method const &method)
{
    ApplicationResult *result = new ApplicationResult(code, method);
    return (result);
}

ApplicationResult *ApplicationResult::from_status_code(StatusCode &code, string method)
{
    ApplicationResult *result = new ApplicationResult(code, method);
    return (result);
}


bool ApplicationResult::is_completed()
{
    return (this->completed);
}


/*
Application *ApplicationResult::app()
{
    return (this->app_);
}
*/

WebservFile *ApplicationResult::file()
{
    return (this->file_);
}


/*
void ApplicationResult::set_app(Application *app)
{
    if(this->app_ == app){
        return ;
    }
    this->app_ = app;
}
*/

void ApplicationResult::set_file(WebservFile *file)
{
    this->file_ = file;
}

/*
std::string const &ApplicationResult::path()
{
    throw std::runtime_error("don't use method()");
}
*/


FileDiscriptor &ApplicationResult::cgi_in()
{
    return (this->cgi_in_);
}

FileDiscriptor &ApplicationResult::cgi_out()
{
    return (this->cgi_out_);
}

ProcessID &ApplicationResult::pid()
{
    return (this->pid_);
}

void ApplicationResult::set_is_cgi(bool flag)
{
    this->is_cgi_ = flag;
}

bool ApplicationResult::is_cgi()
{
    return (this->is_cgi_);
}
