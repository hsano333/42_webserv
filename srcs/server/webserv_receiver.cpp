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

void WebservReceiver::receiver(WebservEvent *event)
{
    DEBUG("WebservReader::read()");
    WebservReadEvent *read_event = static_cast<WebservReadEvent*>(event);
    Request *tmp_req = read_event->req();

    int space = tmp_req->raw_buf_space();
    char* buf = tmp_req->get_raw_buf_pointer();
    FileDiscriptor fd = read_event->fd();
    int sum = 0;
    while(1){
        int tmp = reader->read(fd, &(buf[sum]), space, NULL);
        //cout << "Recv read result=" << tmp << endl;
        if(tmp < 0){
            break;
            ERROR("Receiver Error:" + Utility::to_string(tmp));
            //cout << "Recv read errno=" << errno << endl;
            //cout << "gai_strerror:" << strerror(errno) << endl;
            //cout << "gai_strerror:" << gai_strerror(errno) << endl;
            //break;
            //throw ConnectionException("Read Error");
        }else if(tmp == 0){
            throw ConnectionException("Read Error");
            break;
        }
        sum += tmp;
        space -= tmp;
        if(space <= 0){
            break;
        }
    }
    cout << "read sum:" << sum << endl;
    cout << "read space:" << space << endl;
    buf[sum] = '\0';
    cout << "read buf:[" << buf << "]" << endl;
    tmp_req->renew_raw_buf_space(space);
    tmp_req->increment_raw_buf_size(sum);
    if (sum == 0){
        ERROR("Request data is zero");
        throw HttpException("400");
    }
    //exit(0);
    //io_multi_controller->modify(event->fd(), EPOLLOUT);
}
