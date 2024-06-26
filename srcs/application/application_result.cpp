#include "get_application.hpp"
#include "http_exception.hpp"
#include "normal_reader.hpp"
#include "normal_file.hpp"
#include "directory_file.hpp"
#include "application_result.hpp"
#include <unistd.h>

ApplicationResult::ApplicationResult() : app_(NULL), method_(Method::from_string("NONE")), file_(NULL), completed_(false), is_cgi_(false), reader(NULL), writer(NULL)
{
    ;
}

ApplicationResult::ApplicationResult(StatusCode &code, Method const &method) : app_(NULL), code_(code), method_(method), file_(NULL), completed_(false), is_cgi_(false), reader(NULL), writer(NULL)
{
    ;
}

ApplicationResult::ApplicationResult(StatusCode &code, string &method) : app_(NULL), code_(code), method_(Method::from_string(method)), file_(NULL), completed_(false), is_cgi_(false), reader(NULL), writer(NULL)
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
}

int ApplicationResult::write(char **buf, size_t size)
{
    DEBUG("ApplicationResult::write() size=" + Utility::to_string(size));
    (void)buf;
    (void)size;
    return (this->file_->write(buf, size));
}

int ApplicationResult::close()
{
    return (0);
}

Method const &ApplicationResult::method() const
{
    return (this->method_);
}

StatusCode const &ApplicationResult::status_code() const
{
    return (this->code_);
}

void ApplicationResult::set_status_code(int status_code)
{
    this->code_ = StatusCode::from_int(status_code);
}

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
    this->completed_ = flag;
}

ApplicationResult *ApplicationResult::from_fd(FileDiscriptor const &fd, int in, int out, int pid)
{
    ApplicationResult *result = new ApplicationResult();
    result->cgi_in_ = FileDiscriptor::from_int(in);
    result->cgi_out_ = FileDiscriptor::from_int(out);
    result->pid_ = ProcessID::from_int(pid);

    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    WebservFile *dummy = file_factory->make_dummy_file(fd, result);
    (void)dummy;

    return (result);
}

ApplicationResult *ApplicationResult::from_status_code(FileDiscriptor const &fd, StatusCode &code, Method const &method)
{
    ApplicationResult *result = new ApplicationResult(code, method);

    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    WebservFile *dummy = file_factory->make_dummy_file(fd, result);
    (void)dummy;

    return (result);
}

ApplicationResult *ApplicationResult::from_status_code(FileDiscriptor const &fd, StatusCode &code, string method)
{
    ApplicationResult *result = new ApplicationResult(code, method);

    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    WebservFile *dummy = file_factory->make_dummy_file(fd, result);
    (void)dummy;

    return (result);
}


bool ApplicationResult::completed()
{
    DEBUG("ApplicationResult::completed");
    if(this->file_){
        return (this->file_->completed());
    }
    return (this->completed_);
}


WebservFile *ApplicationResult::file()
{
    return (this->file_);
}

void ApplicationResult::set_file(WebservFile *file)
{
    this->file_ = file;
}

FileDiscriptor &ApplicationResult::cgi_in()
{
    return (this->cgi_in_);
}

FileDiscriptor &ApplicationResult::cgi_out()
{
    return (this->cgi_out_);
}

void ApplicationResult::clear_pid()
{
    this->pid_ = ProcessID();
}

ProcessID ApplicationResult::pid()
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

bool ApplicationResult::is_chunk()
{
    if(this->file_){
        return (this->file_->is_chunk());
    }
    return (false);
}
