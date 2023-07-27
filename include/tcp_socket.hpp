#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include "socket.hpp"
#include <netdb.h>
#include <string>
#include <sys/socket.h>

class TCPSocket : public Socket
{
  public:
    TCPSocket();
    TCPSocket(const TCPSocket& socket);
    TCPSocket& operator=(const TCPSocket& socket);
    ~TCPSocket();

  private:
    void init();
};

#endif
