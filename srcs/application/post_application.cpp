#include "post_application.hpp"
#include "http_exception.hpp"
#include "normal_reader.hpp"
#include "normal_file.hpp"
#include "directory_file.hpp"
#include "connection_exception.hpp"
#include "header_word.hpp"
#include "webserv_io_event.hpp"
#include "socket_chunk_file.hpp"
#include "socket_reader.hpp"
#include "socket_writer.hpp"

PostApplication::PostApplication() : method(Method::from_string("POST"))
{
;
}

PostApplication::~PostApplication()
{
;
}


bool PostApplication::is_cgi() const
{
    return (false);
}

bool PostApplication::check_not_cgi_end(size_t received_size)
{
    (void)received_size;

    return (true);
}

WebservEvent* PostApplication::next_event(WebservEvent *event, WebservEventFactory *event_factory)
{
    DEBUG("PostApplication::next_event");
    WebservFile *file;

    Request *req = event->entity()->request();
    WebservFile *req_file = this->file_factory->make_request_file_read_buf(event->entity()->fd(), req);
    file = this->file_factory->make_socket_file(event->entity()->fd(), req_file, SocketWriter::get_instance(), SocketReader::get_instance());

    if(event->entity()->request()->header().is_chunked()){
        file = this->file_factory->make_socket_chunk_file(event->entity()->fd(), file);
    }

    WebservEvent *new_event = (event_factory->make_making_upload_event(event, file));


    return (new_event);
}

E_EpollEvent PostApplication::epoll_event(WebservEntity *entity)
{
    (void)entity;
    if(entity->io().is_read_completed()){
        DEBUG("PostApplication::epoll_event: EPOLL_NONE");
        return (EPOLL_NONE);
    }else{
        DEBUG("PostApplication::epoll_event: EPOLL_READ");
        return (EPOLL_READ);
    }
}

bool PostApplication::execute(WebservEntity *entity)
{
    entity->io().set_read_completed(true);
    DEBUG("PostApplication::init");
        Header const &header = entity->request()->header();
        std::string const &content_type = header.find(CONTENT_TYPE);

        bool is_chunk = header.is_chunked();
        Body &body = entity->body();

        size_t pos = content_type.find("boundary=");
        if(pos != std::string::npos){
            // 9 is [boundary=] size
            pos += 9;
            body.set_boundary(&(content_type[pos]));
            MYINFO("boundary=" + Utility::to_string(body.boundary()));
        }

        if(is_chunk){
            body.is_chunk = true;
            body.content_length = 0;

        }else{
            body.is_chunk = false;
            std::string const &content_length = header.find(CONTENT_LENGTH);
            if(content_type == header.not_find() && content_length == header.not_find())
            {
                ERROR("Even though it is not chunk, there is neither content-length nor content-type ");
                HttpException("411");
            }

            try{
                if(content_length != header.not_find()){
                    body.content_length = Utility::to_size_t(content_length);
                }
                ssize_t body_size = header.get_content_length();
                if(body_size < 0){
                    body.content_length = body_size;
                }
            }catch (std::invalid_argument &e){
                ERROR("body size is invalid:" + content_length);
                throw HttpException("400");
            }
        }

        if(entity->request()->header().get_content_type().find("text/") != std::string::npos){
            body.is_text= true;
        }else{
            body.is_text= false;
        }

    StatusCode code = StatusCode::from_int(200);
    ApplicationResult *result = ApplicationResult::from_status_code(entity->fd(), code, this->method);
    entity->set_result(result);
    return (true);
}

const Method &PostApplication::which() const
{
    return (this->method);
}


PostApplication *PostApplication::singleton = NULL;
PostApplication *PostApplication::get_instance()
{
    DEBUG("PostApplication::get_instance()");
    if (PostApplication::singleton == NULL){
        singleton = new PostApplication();
    }
    if(singleton->file_factory == NULL){
        ERROR("Not Init");
        throw std::runtime_error("Post Application does't set file_factory");
    }
    return (singleton);
}

PostApplication *PostApplication::get_instance(WebservFileFactory *file_factory)
{
    DEBUG("PostApplication::get_instance()");
    if (PostApplication::singleton == NULL){
        singleton = new PostApplication();
        singleton->file_factory = file_factory;
    }
    return (singleton);
}
