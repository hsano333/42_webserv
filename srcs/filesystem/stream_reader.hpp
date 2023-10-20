#ifndef STREAM_READER_HPP
#define STREAM_READER_HPP
#include "ireader.hpp"
#include "file_discriptor.hpp"

class StreamReader
 : public IReader
{
    public:
        static StreamReader *get_instance();
        ~StreamReader();
        int read(FileDiscriptor fd, char *buf, size_t size, std::ifstream *ifs);
    private:
        static StreamReader *singleton;
        StreamReader();
};

#endif
