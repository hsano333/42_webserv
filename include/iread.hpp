#ifndef SREAD_HPP
#define SREAD_HPP
#include <stdio.h>

class IRead
{
  public:
    IRead();
    virtual ~IRead();
    virtual int iread(int fd, char* buf, size_t size) = 0;
};

class ReadNormal : public IRead
{
  public:
    ReadNormal();
    ~ReadNormal();
    int iread(int fd, char* buf, size_t size);
};

class ReadSocket : public IRead
{
  public:
    ReadSocket();
    ~ReadSocket();
    int iread(int fd, char* buf, size_t size);
};

#endif
