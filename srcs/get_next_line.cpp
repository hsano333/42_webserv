#include "get_next_line.hpp"
#include "utility.hpp"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using std::cout;
using std::endl;
using std::string;

GetNextLine::GetNextLine(int fd_) : _buf_body_pos(NULL), _buf_body_size(0), buf_size(0), _fd(fd_), _sp(NULL), _pos(0)
{
    this->read_line();
}

GetNextLine::~GetNextLine()
{
    delete _sp;
}

int GetNextLine::read(char* buf, int size)
{
    return recv(this->_fd, buf, size, MSG_DONTWAIT);
}

#include <stdio.h>
void GetNextLine::read_line()
{
    int size = this->read(this->_buf, BUF_MAX);
    if (size < 0) {
        cout << "error No.2?" << endl;
        cout << "read_line() test No.0 escape loop() recv error" << endl;
        return;
    }
    this->buf_size = size;
    // if (this->buf_size > 0)
    // break;
    // else if (cnt > 10)
    // return ;
    // usleep(10);
    // cnt++;
    //}

    this->_buf_body_pos = Utility::strnstr(this->_buf, "\r\n\r\n", size);
    // cout << "buf body pos=" << &this->_buf_body_pos << endl;
    // cout << "buf pos=" << &this->_buf << endl;
    if (this->_buf_body_pos) {
        *this->_buf_body_pos = '\0';
        this->_buf_body_pos += 4;
        this->_buf_body_size = size - (this->_buf_body_pos - this->_buf);
    }
    string str = string(this->_buf);
    if (_sp == NULL) {
        this->_sp = new Split(str, "\r\n");
    } else {
        this->_sp->concat(str, "\r\n");
    }
    // if (this->_buf_body_pos == NULL){
    // this->read_line();
    //}
}

size_t GetNextLine::size()
{
    if (this->_sp == NULL)
        return (0);
    return (this->_sp->size());
}

int GetNextLine::get_extra_buf(char* buf)
{
    ssize_t tmp = 0;
    if (this->_buf_body_size > 0) {
        Utility::memcpy(buf, this->_buf_body_pos, this->_buf_body_size);
        buf[0] = '0';
        buf[1] = '1';
        buf[2] = '2';
        buf[3] = '3';
        buf[4] = '4';
        // printf("address=%p\n", this->_buf_body_pos);
        // printf("address1=%p\n", &(this->_buf_body_pos[0]));
        // printf("address2=%p\n", &(this->_buf[0]));
        // cout << "No.2 gnl buf body address 1=" << &(this->_buf_body_pos[0]) << endl;
        tmp = this->_buf_body_size;
        // tmp = 5;
        this->_buf_body_size = 0;
    }
    return (tmp);
    /*
    ssize_t i = 0;
    for(size_t j=0; j < this->sp->size();j++){
        i += ((ssize_t)(*(this->sp))[j].size());
    }
    while(i < this->buf_size)
    {
        if (_buf[i+3] == 10 && _buf[i+2] == 13 && _buf[i+1] == 10 && _buf[i] == 13){
            int size = this->buf_size - (i+4);
            i +=4;
            Utility::memcpy(buf, &(_buf[i]), size);
            this->buf_size = 0;
            return (size);
        }
        i++;
    }
    */
    return (0);
}

string& GetNextLine::getline()
{
    if (this->_sp == NULL || this->_sp->size() == this->_pos) {
        if (this->_buf_body_pos == NULL) {
            this->read_line();
        }
    }
    if (this->_sp == NULL) {
        return (this->last_str);
    }
    if (this->_sp->size() == this->_pos) {
        return (this->last_str);
    }
    return (*(this->_sp))[this->_pos++];
}

int GetNextLine::get_body(char* buf, size_t size)
{
    return (read(buf, size));
}
