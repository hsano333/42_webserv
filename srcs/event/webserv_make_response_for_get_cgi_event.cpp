#include "webserv_make_response_for_get_cgi_event.hpp"
#include "socket_writer.hpp"
#include "socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"
#include "normal_reader.hpp"
#include <assert.h>


WebservMakeResponseForGetCGIEvent::WebservMakeResponseForGetCGIEvent()
{
    ;
}

WebservMakeResponseForGetCGIEvent::~WebservMakeResponseForGetCGIEvent()
{
    ;
}

WebservMakeResponseForGetCGIEvent *WebservMakeResponseForGetCGIEvent::singleton = NULL;
WebservMakeResponseForGetCGIEvent *WebservMakeResponseForGetCGIEvent::get_instance()
{
    if (WebservMakeResponseForGetCGIEvent::singleton == NULL){
        singleton = new WebservMakeResponseForGetCGIEvent();
    }
    return (singleton);
}

void WebservMakeResponseForGetCGIEvent::delete_myself()
{
    if(WebservMakeResponseForGetCGIEvent::singleton)
        delete WebservMakeResponseForGetCGIEvent::singleton;
}

WebservEvent *WebservMakeResponseForGetCGIEvent::from_event(WebservEvent *event)
{
    DEBUG("WebservMakeResponseForGetCGIEvent::from_fd");
    WebservMakeResponseForGetCGIEvent *res_event = WebservMakeResponseForGetCGIEvent::get_instance();
    WebservEvent *new_event = new WebservEvent( res_event, io_work_reverse_io<WebservMakeResponseForGetCGIEvent>, event->entity());
    //new_event->entity()->io().set_source(src);
    //new_event->entity()->io().set_destination(dst);
    //WebservMakeResponseForGetCGIEvent *io_event = WebservMakeResponseForGetCGIEvent::get_instance();
    //WebservEvent *new_event =  new WebservEvent( io_event, dummy_func<WebservMakeResponseForGetCGIEvent>, event->entity());
    return (new_event);
}

/*
WebservEvent *WebservMakeResponseForGetCGIEvent::from_fd(FileDiscriptor &write_fd, FileDiscriptor &read_fd, WebservFile *read_src, WebservFile *read_dst, WebservFile *write_src, WebservFile *write_dst, WebservEvent * event)
{
    DEBUG("WebservMakeResponseForGetCGIEvent::from_fd");
    WebservMakeResponseForGetCGIEvent *io_event = WebservMakeResponseForGetCGIEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, dummy_func<WebservMakeResponseForGetCGIEvent>, event->entity());
    new_event->entity()->io().set_read_io(read_src, read_dst);
    new_event->entity()->io().set_write_io(write_src, write_dst);

    new_event->entity()->io().set_read_fd(read_fd);
    new_event->entity()->io().set_write_fd(write_fd);
    new_event->entity()->io().switching_io(EPOLLIN);
    return (new_event);
}
*/

Response* WebservMakeResponseForGetCGIEvent::make_response_for_cgi(ApplicationResult *result, WebservEntity *entity)
{
    DEBUG("WebservMakeResponseForGetCGIEvent::make_response_for_cgi()");

    char *data;
    int read_size = result->file()->read(&data, MAX_READ_SIZE);

    if(read_size <= 0){
        ERROR("CGI Respons Data is nothing or error");
        throw HttpException("500");
    }

    cout << "cgi read_size:" << read_size << endl;
    cout << "cgi data:[" <<  data <<  "]" << endl;
    //cout << "cgi body_p:[" << body_p <<  "]" << endl;


    printf("data=[[[[[[[[");
    for(int i=0;i<read_size;i++){
        if((data)[i] == '\r'){
            printf("R");
        }else if((data)[i] == '\n'){
            printf("N");
        }else{
            printf("%c", (data)[i]);
        }
    }
    printf("]\nbuffer size =[%d]\n\n", read_size);




    // RFC3875にはNL(New Line)の具体的な定義がないため、
    // 改行文字はLFとする(Linuxでの運用を想定しているため)
    char *body_p = Utility::strnstr(data, LF2, read_size);
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


    Response *res = Response::from_cgi_header_line(entity->fd(), headers_line, file);

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

WebservEvent* WebservMakeResponseForGetCGIEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservMakeResponseForGetCGIEvent::make_next_event()");
    WebservEntity *entity = event->entity();
    ApplicationResult *result = event->entity()->app_result();
    Response *res = make_response_for_cgi(result, entity);
    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    WebservFile *res_file = file_factory->make_webserv_file_regular(event->entity()->fd(), res);

    entity->set_response(res);

    //WebservFile *read_dst = entity->io().destination_for_read();
    WebservFile *read_dst = file_factory->make_socket_file(event->entity()->fd(), event->entity()->fd(), SocketWriter::get_instance(), NULL);
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
        //DEBUG("WebservMakeResponseForGetCGIEvent::make_next_event() No.1");
        //return (event_factory->make_making_response_event(event, event->entity()->io().destination_for_read()));
    //}
    //return (event_factory->make_waiting_out_cgi(event, write_src, read_dst, result));

    return (event_factory->make_io_socket_for_get_cgi(event));
    //return (event);
}

E_EpollEvent WebservMakeResponseForGetCGIEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    DEBUG("WebservMakeResponseForGetCGIEvent::epoll_event()");

    if (event->entity()->completed()){
        DEBUG("WebservMakeResponseForGetCGIEvent::epoll_event() No.1");
        return (EPOLL_WRITE);
    }
    //return (EPOLL_NONE);
    return (EPOLL_FOR_CGI_WAIT_CGI);

    /*
    if (event->entity()->completed()){
        DEBUG("WebservMakeResponseForGetCGIEvent::epoll_event() No.1");
        return (EPOLL_NONE);
    }
    return (EPOLL_FOR_CGI);


    DEBUG("WebservMakeResponseForGetCGIEvent::epoll_event()");
    //return (EPOLL_FOR_CGI);
    if(event->entity()->io().in_out() == EPOLLIN){
        if (event->entity()->completed()){
            DEBUG("WebservMakeResponseForGetCGIEvent::epoll_event() No.1");
            return (EPOLL_NONE);
        }else{
            DEBUG("WebservMakeResponseForGetCGIEvent::epoll_event() No.2");
            return (EPOLL_READ);
        }
    }
            DEBUG("WebservMakeResponseForGetCGIEvent::epoll_event() No.3");
    return (EPOLL_NONE);
    */
}

void WebservMakeResponseForGetCGIEvent::check_completed(WebservEntity * entity)
{
    WebservFile *dst = entity->io().destination_for_write();
    bool flag = dst->completed();
    DEBUG("WebservMakeResponseForGetCGIEvent::check_completed flag:" + Utility::to_string(flag));
    //if(flag == false){

    //}
    entity->set_completed(flag);
}
