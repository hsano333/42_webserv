#include "socket_data.hpp"
#include "config.hpp"
#include <iostream>

using std::cout;
using std::endl;
using std::string;

SocketData::SocketData() : _timeout_cnt(0){}

//SocketData::SocketData() :  {}
SocketData::~SocketData() {}

bool SocketData::increment_timeout(int time)
{
    this->_timeout_cnt = this->_timeout_cnt + time;
    if (_timeout_cnt > _timeout) {
        return (true);
    }
    return (false);
}

void SocketData::clear_timeout()
{
    _timeout_cnt = 0;
}

/*
Config const* SocketData::get_config() const
{
    return _config;
}
*/
