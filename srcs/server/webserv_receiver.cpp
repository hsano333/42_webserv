#include "webserv_receiver.hpp"

WebservReceiver::WebservReceiver(
            IOMultiplexing *io_multi_controller,
            EventManager *event_manager
        ):
        io_multi_controller(io_multi_controller),
        event_manager(event_manager)

{
    ;
}

WebservReceiver::~WebservReceiver()
{
    ;
}

void WebservReceiver::read(WebservEvent *event)
{
    DEBUG("WebservReader::read()");
    WebservReadEvent *read_event = static_cast<WebservReadEvent*>(event);
    Request *tmp_req = read_event->req();

    int space = tmp_req->raw_buf_space();
    //cout << "spalce:" << space<< endl;
    char* buf = tmp_req->get_raw_buf_pointer();
    //for(size_t i=0;i<512;i++){
        //buf[i] = i;
    //}
    int sum = 0;
    //size_t size = 0;
    while(1){
        int tmp = read_event->read(&(buf[sum]), space);
        if(tmp <= 0){
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
    //exit(0);
    //io_multi_controller->modify(event->get_fd(), EPOLLOUT);
}
