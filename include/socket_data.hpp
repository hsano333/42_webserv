#ifndef SOCKET_DATA_HPP
#define SOCKET_DATA_HPP

#include "split.hpp"
// #include "soket_data.hpp"
#include <map>
#include <string>

using std::string;

class SocketData
{
  public:
    SocketData();
    ~SocketData();

    bool increment_timeout(int time);
    void clear_timeout();

  protected:
    const static int _timeout = 200;
    const static int BUF_MAX = 1600;
    int _timeout_cnt;

};

#endif
