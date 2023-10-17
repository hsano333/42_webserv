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
        int read(FileDiscriptor fd, char *buf, size_t size);
    private:
        static NormalReader *singleton;
        NormalReader();
};

#endif
