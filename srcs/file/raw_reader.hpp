#ifndef RAW_FILE_LOADER_HPP
#define RAW_FILE_LOADER_HPP
#include "ireader.hpp"
#include "filepath.hpp"
#include "file_discriptor.hpp"

class RawReader : public IReader
{
    public:
        RawReader();
        ~RawReader();
        int read(Filepath &filepath, char *data, size_t size);
        int read(FileDiscriptor const &fd, char *data, size_t size);
    private:
        FileDiscriptor fd;
        std::string filepath;
};

#endif
