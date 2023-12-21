#include "webserv_receiver.hpp"
#include "http_exception.hpp"
#include "connection_exception.hpp"
#include <string.h>

WebservReceiver::WebservReceiver(
            IOMultiplexing *io_multi_controller,
            EventManager *event_manager,
            SocketReader *reader
        ):
        io_multi_controller(io_multi_controller),
        event_manager(event_manager),
        reader(reader)

{
    ;
}

WebservReceiver::~WebservReceiver()
{
    ;
}

void WebservReceiver::recv(WebservEvent *event)
{
    DEBUG("WebservReader::read()");
    WebservReadEvent *read_event = static_cast<WebservReadEvent*>(event);
    Request *tmp_req = read_event->req();
    cout << "test No.1" << endl;
    HttpData *source = read_event->source();
    cout << "test No.2" << endl;
    if(source == NULL){
        ERROR("WebservReceiver::recv():  source is NULL");
        throw HttpException("500");
    }
    cout << "test No.3" << endl;
    source->open_source_file();
    cout << "test No.4" << endl;

    int space = tmp_req->raw_buf_space();
    char* buf = tmp_req->get_raw_buf_pointer();
    cout << "test No.6" << endl;
    FileDiscriptor fd = read_event->fd();
    size_t sum = tmp_req->buf_size();
    MYINFO("Receiver  fd:" + Utility::to_string(fd.to_int()));
    MYINFO("Receiver  sum:" + Utility::to_string(sum));
    MYINFO("Receiver space:" + Utility::to_string(space));
    //space = 100;
    event->set_completed(false);
    while(1){
        char* p_data = &(buf[sum]);
        ssize_t tmp = source->get_data(&p_data);

        MYINFO("Receiver read < 0:" + Utility::to_string(tmp));
        if(tmp < 0){
            MYINFO("Receiver read < 0:" + Utility::to_string(tmp));
            event->set_completed(true);
            break;
        }else if(tmp == 0){
            ERROR("Read Connection Error");
            throw ConnectionException("Read Error");
        }
        sum += tmp;
        space -= tmp;
        if(space <= 0){
            tmp_req->set_read_completed(false);
            break;
        }
    }
    MYINFO("Receive data size::" + Utility::to_string(sum));
    cout << "read sum:" << sum << endl;
    cout << "read space:" << space << endl;
    buf[sum] = '\0';
    cout << "read buf:[" << buf << "]" << endl;
    tmp_req->set_buf_pos(sum);
    //tmp_req->decrement_raw_buf_size(sum);
    if (sum == 0){
        ERROR("Request data is zero");
        throw HttpException("400");
    }


    char *body_p = Utility::strnstr(tmp_req->buf(), "\r\n\r\n", (size_t)tmp_req->buf_size());

    if(body_p == NULL && tmp_req->buf_size() >= MAX_READ_SIZE){
        body_p = Utility::strnstr(tmp_req->buf(), "\r\n",(size_t)tmp_req->buf_size());
        if(body_p){
            ERROR("Invalid Request. Heade is too long ");
            throw HttpException("431");
        }
        ERROR("Invalid Request.  request line is too long ");
        throw HttpException("414");
    }
    //cout << "req->buf_size():" << req->buf_size() << endl;
    //char *buf_p = req->buf();
    //buf_p[req->buf_size()] = '\0';
    //printf("buf_p=%s\n", buf_p);
    if(body_p == NULL){
        DEBUG("WebservParser:: not still read Request header");
        event->set_completed(false);
        return ;
    }

    // bodyの開始位置の記録
    body_p += 4;
    tmp_req->set_buf_body(body_p, tmp_req->buf_size() - (size_t)(body_p - tmp_req->buf()));

    // parser処理でbodyを読み込まないため
    body_p -= 4;
    *body_p = '\0';


    event->set_completed(true);

    //exit(0);
    //io_multi_controller->modify(event->fd(), EPOLLOUT);
}

