#include "webserv_make_response_for_post_cgi_event.hpp"
#include "socket_writer.hpp"
#include "socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"
#include "normal_reader.hpp"
#include <assert.h>


WebservMakeResponseForPostCGIEvent::WebservMakeResponseForPostCGIEvent()
{
    ;
}

WebservMakeResponseForPostCGIEvent::~WebservMakeResponseForPostCGIEvent()
{
    ;
}

WebservMakeResponseForPostCGIEvent *WebservMakeResponseForPostCGIEvent::singleton = NULL;
WebservMakeResponseForPostCGIEvent *WebservMakeResponseForPostCGIEvent::get_instance()
{
    if (WebservMakeResponseForPostCGIEvent::singleton == NULL){
        singleton = new WebservMakeResponseForPostCGIEvent();
    }
    return (singleton);
}

WebservEvent *WebservMakeResponseForPostCGIEvent::from_event(WebservEvent *event)
{
    DEBUG("WebservMakeResponseForPostCGIEvent::from_fd");
    WebservMakeResponseForPostCGIEvent *res_event = WebservMakeResponseForPostCGIEvent::get_instance();
    WebservEvent *new_event = new WebservEvent( res_event, io_work_reverse_io<WebservMakeResponseForPostCGIEvent>, event->entity());
    //new_event->entity()->io().set_source(src);
    //new_event->entity()->io().set_destination(dst);
    //WebservMakeResponseForPostCGIEvent *io_event = WebservMakeResponseForPostCGIEvent::get_instance();
    //WebservEvent *new_event =  new WebservEvent( io_event, dummy_func<WebservMakeResponseForPostCGIEvent>, event->entity());
    return (new_event);
}

/*
WebservEvent *WebservMakeResponseForPostCGIEvent::from_fd(FileDiscriptor &write_fd, FileDiscriptor &read_fd, WebservFile *read_src, WebservFile *read_dst, WebservFile *write_src, WebservFile *write_dst, WebservEvent * event)
{
    DEBUG("WebservMakeResponseForPostCGIEvent::from_fd");
    WebservMakeResponseForPostCGIEvent *io_event = WebservMakeResponseForPostCGIEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, dummy_func<WebservMakeResponseForPostCGIEvent>, event->entity());
    new_event->entity()->io().set_read_io(read_src, read_dst);
    new_event->entity()->io().set_write_io(write_src, write_dst);

    new_event->entity()->io().set_read_fd(read_fd);
    new_event->entity()->io().set_write_fd(write_fd);
    new_event->entity()->io().switching_io(EPOLLIN);
    return (new_event);
}
*/

Response* WebservMakeResponseForPostCGIEvent::make_response_for_cgi(ApplicationResult *result, WebservEntity *entity)
{
    DEBUG("WebservMakeResponseForPostCGIEvent::make_response_for_cgi()");

    char *data;
    int read_size = result->file()->read(&data, MAX_READ_SIZE);

    if(read_size <= 0){
        ERROR("CGI Respons Data is nothing or error");
        throw HttpException("500");
    }

    //todo
    //waitpid();

    // RFC3875にはNL(New Line)の具体的な定義がないため、
    // 改行文字はLFとする(Linuxでの運用を想定しているため)
    data[read_size] = '\0';
    char *body_p = Utility::strnstr(data, NL2_CGI, read_size);
    DEBUG("cgi read data=" + std::string(data));
    if(!body_p){
        ERROR("CGI Statement Error: not find LFLF(double New Line)");
        throw HttpException("500");
    }

    size_t header_size = body_p - data;
    if(header_size >= MAX_REAUEST_EXCEPT_BODY){
        ERROR("exceed CGI Response Status Line" + Utility::to_string(header_size));
        throw HttpException("exceed CGI Response Status Line:" + Utility::to_string(header_size));
    }
    *body_p = '\0';
    cout << "cgi header_size:" << header_size << endl;
    cout << "cgi data:[" <<  data <<  "]" << endl;
    cout << "cgi body_p:[" << body_p <<  "]" << endl;

    Split headers_line(data, LF, false, true);
    for(size_t i=0;i<headers_line.size();i++){
        cout << "headers_line[" << i << "]=" << headers_line[i] << endl;
    }
    IReader *reader = NormalReader::get_instance();

    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    //WebservFile *file = file_factory->make_socket_file(result->cgi_out(), NULL, reader );
    WebservFile *file = file_factory->make_pipe_file(entity->fd(),  result->cgi_out(), reader);
    Request const *req = entity->request();
    ConfigServer const *server = entity->config()->get_server(req);
    Response *res = Response::from_cgi_header_line(headers_line, file);

    int tmp_size = read_size - header_size - 2;
    cout << "body_p[2+i]:[" ;
    for(int i=0;i<tmp_size;i++){
        cout << body_p[2+i];
    }
    cout << "]" << endl;
    res->set_buf_body(body_p + 2, tmp_size);
    if(!res->check_body_size(server))
    {
        delete res;
        throw HttpException("500");
    }
    if(res->header().get_content_length() == -1)
    {
        if(res->header().is_chunked() == false){
            res->add_header(TRANSFER_ENCODING, TRANSFER_ENCODING_CHUNKED);
        }
    }

    res->check_body_and_chunk();
    if(!res->has_body()){
        res->add_header(CONTENT_LENGTH, "0");
    }
    if(res->is_chunk()){
        WebservFileFactory *file_factory = WebservFileFactory::get_instance();
        WebservFile *socket_file = file_factory->make_socket_chunk_file_for_write(entity->fd(), res->get_file(), res->buffer());
        res->clear_buffer();
        socket_file->set_chunk(true);
        res->switching_file(socket_file);
    }
    return (res);
}



WebservEvent* WebservMakeResponseForPostCGIEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservMakeResponseForPostCGIEvent::make_next_event()");
    WebservEntity *entity = event->entity();
    ApplicationResult *result = event->entity()->app_result();
    Response *res = make_response_for_cgi(result, entity);
    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    WebservFile *res_file = file_factory->make_webserv_file_regular(event->entity()->fd(), res);

    entity->set_response(res);
    //WebservFile *read_dst = entity->io().destination_for_read();
    WebservFile *read_dst = file_factory->make_socket_file(event->entity()->fd(), SocketWriter::get_instance(), NULL);
    entity->io().set_write_io(res_file, read_dst);
    //read_dst->open();
    /*

    char buf[2000]={0};
    char *buf_p = &(buf[0]);
    int read_result = res_file->read(&buf_p, 1000);
    int write_result = 0;
    if(read_result > 0){
        write_result = read_dst->write(&buf_p, read_result);
    }
    printf("result=%d, buf_p=[%s], write_result=%d\n", read_result, buf_p, write_result);
    read_result = res_file->read(&buf_p, 1000);
    if(read_result > 0){
        write_result = read_dst->write(&buf_p, read_result);
    }
    printf("result=%d, buf_p=[%s], write_result=%d\n", read_result, buf_p, write_result);
    read_result = res_file->read(&buf_p, 1000);
    if(read_result > 0){
        write_result = read_dst->write(&buf_p, read_result);
    }
    printf("result=%d, buf_p=[%s], write_result=%d\n", read_result, buf_p, write_result);
    read_result = res_file->read(&buf_p, 1000);
    if(read_result > 0){
        write_result = read_dst->write(&buf_p, read_result);
    }
    printf("result=%d, buf_p=[%s], write_result=%d\n", read_result, buf_p, write_result);
    */


    //entity->io().set_read_fd(res->get_file()->fd());

    //if(event->entity()->io().in_out() == EPOLLIN){
        //DEBUG("WebservMakeResponseForPostCGIEvent::make_next_event() No.1");
        //return (event_factory->make_making_response_event(event, event->entity()->io().destination_for_read()));
    //}
    //return (event_factory->make_waiting_out_cgi(event, write_src, read_dst, result));

    //event->entity()->io().switching_io(EPOLLIN);
    return (event_factory->make_io_socket_for_post_cgi(event));
    //return (event);
}

E_EpollEvent WebservMakeResponseForPostCGIEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    DEBUG("WebservMakeResponseForPostCGIEvent::epoll_event()");

    if (event->entity()->completed()){
        DEBUG("WebservMakeResponseForPostCGIEvent::epoll_event() No.1");
        return (EPOLL_WRITE);
    }
    return (EPOLL_NONE);

    /*
    if (event->entity()->completed()){
        DEBUG("WebservMakeResponseForPostCGIEvent::epoll_event() No.1");
        return (EPOLL_NONE);
    }
    return (EPOLL_FOR_CGI);


    DEBUG("WebservMakeResponseForPostCGIEvent::epoll_event()");
    //return (EPOLL_FOR_CGI);
    if(event->entity()->io().in_out() == EPOLLIN){
        if (event->entity()->completed()){
            DEBUG("WebservMakeResponseForPostCGIEvent::epoll_event() No.1");
            return (EPOLL_NONE);
        }else{
            DEBUG("WebservMakeResponseForPostCGIEvent::epoll_event() No.2");
            return (EPOLL_READ);
        }
    }
            DEBUG("WebservMakeResponseForPostCGIEvent::epoll_event() No.3");
    return (EPOLL_NONE);
    */
}

void WebservMakeResponseForPostCGIEvent::check_completed(WebservEntity * entity)
{
    //todo
    DEBUG("WebservMakeResponseForPostCGIEvent::check_completed");
    //entity->set_completed(true);
    WebservFile *dst = entity->io().destination_for_write();
    bool flag = dst->completed();
    DEBUG("WebservMakeResponseForPostCGIEvent::check_completed flag:" + Utility::to_string(flag));
    entity->set_completed(flag);
    return;

    //WebservFile *dst = entity->io().destination_for_read();
    //bool flag = dst->completed();
    //DEBUG("WebservMakeResponseForPostCGIEvent::check_completed flag:" + Utility::to_string(flag));
    //entity->set_completed(flag);
}
