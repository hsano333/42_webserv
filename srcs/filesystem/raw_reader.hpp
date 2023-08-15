#ifndef RAW_FILE_LOADER_HPP
#define RAW_FILE_LOADER_HPP
#include "ireader.hpp"
#include "filepath.hpp"

class RawReader : public IReader
{
    public:
        RawReader();
        ~RawReader();
        int read(Filepath &filepath, char *data);
    private:
        int fd;
        std::string filepath;
};

#endif
