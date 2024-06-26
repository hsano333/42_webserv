#ifndef NORMAL_READER_HPP
#define NORMAL_READER_HPP
#include "ireader.hpp"
#include "file_discriptor.hpp"

class NormalReader
 : public IReader
{
    public:
        static NormalReader *get_instance();
        ~NormalReader();
        int read(FileDiscriptor const &fd, char *buf, size_t size, std::fstream *ifs);
    private:
        static NormalReader *singleton;
        NormalReader();
};

#endif
