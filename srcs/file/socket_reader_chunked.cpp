
#include "socket_reader_chunked.hpp"
#include "connection_exception.hpp"
#include "global.hpp"
#include "http_exception.hpp"
#include "utility.hpp"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

SocketReaderChunked::SocketReaderChunked()
{
    DEBUG("SocketReaderChunked::SocketReaderChunked()");
}
SocketReaderChunked::~SocketReaderChunked()
{
}


SocketReaderChunked* SocketReaderChunked::singleton = NULL;
SocketReaderChunked *SocketReaderChunked::get_instance()
{
    if (SocketReaderChunked::singleton == NULL){
        singleton = new SocketReaderChunked();
    }
    return (singleton);
}


int SocketReaderChunked::read(FileDiscriptor const &fd, char *buf, size_t size, std::fstream *ifs)
{
    (void)ifs;
    (void)buf;

    char tmp_buf[21] = {0};
    ssize_t read_size = ::recv(fd.to_int(), tmp_buf, size, 20);
    char *pos = Utility::strnstr(tmp_buf, CRLF, read_size);
    int exceed_str_size = 20 - (pos - &(tmp_buf[0])+2);
    if(pos){
        pos[0] = '\0';
    }else{
        ERROR(" not chunked format");
        throw HttpException("400");
    }
    size_t chunked_size = Utility::hex_string_to_number(pos);
    if(chunked_size == 0){
        return (0);
    }
    pos += 2;
    Utility::memcpy(buf, pos, exceed_str_size);
    buf += exceed_str_size;

    ssize_t read_size = ::recv(fd.to_int(), buf, size, MSG_DONTWAIT);


    if(read_size == 0){
        ERROR("Client Read Close");
        throw ConnectionException("Client Read Close");
    }
    return (read_size);
}
