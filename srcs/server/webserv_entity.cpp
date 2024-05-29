#include "webserv_entity.hpp"
#include "request.hpp"
#include "response.hpp"

WebservEntity::WebservEntity() : req_(NULL), res_(NULL), cfg_(NULL), app_(NULL), app_result_(NULL), completed_(false), force_close_(false),event_error_(None), error_code_(StatusCode::from_int(0))
{
    DEBUG("WebservEntity() Constructor");
}

WebservEntity::~WebservEntity()
{
    DEBUG("WebservEntity() Destructor address:" + Utility::to_string(this));
    /*
    DEBUG("WebservEntity() Destructor request address:" + Utility::to_string(this->req_));
    if(this->req_)
        delete this->req_;
    DEBUG("WebservEntity() Destructor No.1 response address:" + Utility::to_string(this->res_));
    if(this->res_)
        delete this->res_;
    DEBUG("WebservEntity() Destructor No.2");
    */
    DEBUG("WebservEntity() Destructor No.1 response address:" + Utility::to_string(this->app_result_));
    //if(this->app_result_)
        //delete this->app_result_;
    //delete this->app_;
}


// fd,socket_fdはコピーする
WebservEntity::WebservEntity(FileDiscriptor &fd, FileDiscriptor &socket_fd, Config *cfg) : fd_(fd), sock_fd_(socket_fd), req_(NULL), res_(NULL), cfg_(cfg), app_(NULL), app_result_(NULL), completed_(false), force_close_(false),event_error_(None)
{
    DEBUG("WebservEntity::WebservEntity Constructor from fd");
}

WebservEntity::WebservEntity(WebservEntity const &entity) : fd_(entity.fd_), sock_fd_(entity.sock_fd_), req_(entity.req_), res_(entity.res_), cfg_(entity.cfg_), app_(entity.app_), app_result_(entity.app_result_), completed_(entity.completed_), force_close_(entity.force_close_),event_error_(None)
{
    DEBUG("WebservEntity() Copy Constructor");
}


WebservEntity& WebservEntity::operator=(WebservEntity const &entity)
{
    ERROR("Copying of WebservEntity is prohibited");
    throw std::runtime_error("Copying of WebservEntity is prohibited");
    if(this == &entity){
        return (*this);
    }

    this->fd_ = entity.fd_;
    this->req_ = entity.req_;
    this->res_ = entity.res_;
    this->cfg_ = entity.cfg_;
    this->app_ = entity.app_;
    this->app_result_ = entity.app_result_;
    this->completed_ = entity.completed_;
    this->force_close_ = entity.force_close_;
    return (*this);
}

void WebservEntity::set_app(Application *app)
{
    if(this->app_ == app){
        return ;
    }
    this->app_ = app;
}

void WebservEntity::set_result(ApplicationResult *result)
{
    if(this->app_result_ == result){
        return ;
    }
    if(this->app_result_){
        // must not delete because of double free;
        //delete this->app_result_;

    }
    this->app_result_ = result;
}

void WebservEntity::set_request(Request *req)
{
    if(this->req_ == req){
        return ;
    }
    if(this->req_ != NULL){
        // must not delete because of double free;
        //delete this->req_;
    }
    this->req_ = req;
}

void WebservEntity::set_response(Response *res)
{
    DEBUG("WebservEntity::set_response address:" + Utility::to_string(res));
    if(this->res_ == res){
        return ;
    }
    DEBUG("WebservEntity::set_response No.1 address:" + Utility::to_string(res));
    if(this->res_){
        // must not delete because of double free;
        //delete this->res_;
    }
    this->res_ = res;
}


FileDiscriptor const &WebservEntity::fd()
{
    return (this->fd_);
}

FileDiscriptor const &WebservEntity::socket_fd()
{
    return (this->sock_fd_);
}

Application *WebservEntity::app()
{
    return (this->app_);
}

ApplicationResult *WebservEntity::app_result()
{
    return (this->app_result_);
}

Request *WebservEntity::request()
{
    return (this->req_);
}

Response *WebservEntity::response()
{
    return (this->res_);
}

WebservIO &WebservEntity::io()
{
    return (this->io_);
}


Config const *WebservEntity::config()
{
    return (this->cfg_);
}


void WebservEntity::set_completed(bool flag)
{
    DEBUG("WebservEntity::set_completed:" + Utility::to_string(flag));
    this->completed_ = flag;
}

bool WebservEntity::force_close()
{
    return (this->force_close_);
}

Body &WebservEntity::body()
{
    return (this->body_);
}

void WebservEntity::set_body(Body &body)
{
    this->body_ = body;
}

void WebservEntity::set_force_close(bool flag)
{
    this->force_close_ = flag;
}

EventError WebservEntity::event_error()
{
    return (this->event_error_);
}

void WebservEntity::set_event_error(EventError error)
{
    this->event_error_ = error;
}

void WebservEntity::set_error_code(StatusCode &code)
{
    this->error_code_ = code;
}

StatusCode &WebservEntity::error_code()
{
    return (this->error_code_);
}

