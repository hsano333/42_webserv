#ifndef READ_RAW_HPP
#define READ_RAW_HPP
#include "ireader.hpp"
#include "file_discriptor.hpp"

class ReadRaw : public IReader
{
    public:
        ReadRaw();
        ~ReadRaw();
        int read(FileDiscriptor fd, char *buf, size_t size);
};

#endif
