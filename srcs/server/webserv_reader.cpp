#include "webserv_reader.hpp"

WebservReader::WebservReader(
            IOMultiplexing *io_multi_controller,
            EventManager *event_manager
        ):
        io_multi_controller(io_multi_controller),
        event_manager(event_manager)

{
    ;
}

WebservReader::~WebservReader()
{
    ;
}

void WebservReader::read(WebservEvent *event)
{
    DEBUG("WebservReader::read()");
    WebservReadEvent *read_event = static_cast<WebservReadEvent*>(event);
    cout << "test No.1" << endl;
    Request *tmp_req = read_event->req();
    cout << "test No.2" << endl;
    //(void)event;
    //event.ireader.read();
    //char buf[MAX_BUF+1];
    //int size = 0;
    int space = tmp_req->raw_buf_space();
    cout << "test No.3" << endl;
    //cout << "spalce:" << space<< endl;
    char* buf = tmp_req->get_raw_buf_pointer();
    for(size_t i=0;i<512;i++){
        buf[i] = i;
    }
    cout << "test No.4" << endl;
    printf("buf No.1 =%p\n", buf);
    int sum = 0;
    //size_t size = 0;
    while(1){
        cout << "sum:" << sum << endl;
        cout << "spalce:" << space<< endl;
        int tmp = read_event->read(&(buf[sum]), space);
        cout << "test No.5" << endl;
        if(tmp <= 0){
            break;
        }
        sum += tmp;
        space -= tmp;
        if(space <= 0){
            break;
        }
    }
    buf[sum] = '\0';
    cout << "buf:" << buf << endl;
    tmp_req->renew_raw_buf_space(space);
    //io_multi_controller->modify(event->get_fd(), EPOLLOUT);
}
