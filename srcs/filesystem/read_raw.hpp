#ifndef READ_RAW_HPP
#define READ_RAW_HPP
#include "ireader.hpp"

class ReadRaw : public IReader
{
    public:
        ReadRaw();
        virtual ~ReadRaw();
        int iread(int fd, char *buf);
};

#endif
