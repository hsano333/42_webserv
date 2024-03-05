#ifndef STREAM_WRITER_HPP
#define STREAM_WRITER_HPP
#include "iwriter.hpp"
#include "file_discriptor.hpp"

class StreamWriter
 : public IWriter
{
    public:
        static StreamWriter *get_instance();
        ~StreamWriter();
        int write(FileDiscriptor const &fd, char const *buf, size_t size, std::fstream *ofs);
    private:
        static StreamWriter *singleton;
        StreamWriter();
};

#endif
