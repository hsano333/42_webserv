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

    int space = tmp_req->raw_buf_space();
    char* buf = tmp_req->get_raw_buf_pointer();
    FileDiscriptor fd = read_event->fd();
    size_t sum = tmp_req->buf_size();
    MYINFO("Receiver  fd:" + Utility::to_string(fd.to_int()));
    MYINFO("Receiver  sum:" + Utility::to_string(sum));
    MYINFO("Receiver space:" + Utility::to_string(space));
    //space = 100;
    while(1){
        int tmp = reader->read(fd, &(buf[sum]), space, NULL);
        MYINFO("Receiver read < 0:" + Utility::to_string(tmp));
        if(tmp < 0){
            MYINFO("Receiver read < 0:" + Utility::to_string(tmp));
            event->set_completed(false);
            tmp_req->set_read_completed(true);
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
    //exit(0);
    //io_multi_controller->modify(event->fd(), EPOLLOUT);
}

