#ifndef NORMAL_FILE_HPP
#define NORMAL_FILE_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "ireader.hpp"
#include "iwriter.hpp"
#include "file_discriptor.hpp"
#include "file.hpp"

typedef enum E_RWOption
{
    READ_ONLY,
    WRITE_ONLY,
}RWOption;

/*
typedef enum E_FileState
{
    FILE_NOT_OPEN,
    FILE_OPEN,
    FILE_CLOSE,
    FILE_ERROR,
}FileState;
*/

class NormalFile : public File
{
    public:
        ~NormalFile();
        //static NormalFile* from_filepath(IReader* iread, std::string &filepath, RWOption option);
        static NormalFile* from_filepath(std::string const &filepath, std::ios_base::openmode);
        int open();
        int close();
        int read(char **buf, size_t size);
        int write(char **buf, size_t size);
        int save(char *data, size_t size);
        bool can_read();
        bool can_write();
        bool is_chunk();
        size_t size();
        int remove();
        std::string const &path();

    private:
        NormalFile();
        //NormalFile(IReader* iread, std::string const &filepath, RWOption option);
        NormalFile(IReader* iread, std::string const &filepath, std::ios_base::openmode option);
        NormalFile(IWriter* iwriter, std::string const &filepath, std::ios_base::openmode option);
        IReader *ireader;
        IWriter *iwriter;
        std::string filepath;
        //RWOption    option;
        std::ios_base::openmode option;
        FileDiscriptor fd;
        FileState   state;
        std::fstream iofs;
        std::vector<char> tmp_buf;

};

#endif
