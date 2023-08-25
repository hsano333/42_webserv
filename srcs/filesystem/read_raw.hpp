#ifndef READ_RAW_HPP
#define READ_RAW_HPP
#include "ireader.hpp"
#include "file_discriptor.hpp"

class ReadRaw : public IReader
{
    public:
        ReadRaw();
        virtual ~ReadRaw();
        int iread(FileDiscriptor fd, char *buf);
};

#endif
