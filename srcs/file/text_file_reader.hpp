#ifndef TEXT_FILE_LOADER_HPP
#define TEXT_FILE_LOADER_HPP
#include "ifile_reader.hpp"
#include "filepath.hpp"
#include "global.hpp"

class TextFileReader : IFileReader
{
    public:
        TextFileReader();
        ~TextFileReader();
        int read_file(Filepath &filepath, char *data);
    private:
};

#endif
