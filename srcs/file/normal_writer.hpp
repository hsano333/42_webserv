#ifndef NORMAL_WRITER_HPP
#define NORMAL_WRITER_HPP
#include "iwriter.hpp"
#include "file_discriptor.hpp"
#include <unistd.h>

class  NormalWriter: public IWriter
{
    public:
        static NormalWriter *get_instance();
        ~NormalWriter();
        int write(FileDiscriptor const &fd, char const *buf, size_t size, std::fstream *ifs);
    private:
        static NormalWriter *singleton;
        NormalWriter();
};

#endif
