#ifndef DIRECTORY_READER_HPP
#define DIRECTORY_READER_HPP
#include "ireader.hpp"
#include "file_discriptor.hpp"

class DirectoryReader
 : public IReader
{
    public:
        static DirectoryReader *get_instance();
        ~DirectoryReader();
        int read(FileDiscriptor fd, char *buf, size_t size);
    private:
        static DirectoryReader *singleton;
        DirectoryReader();
};

#endif
