#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include "ireader.hpp"
#include "file_discriptor.hpp"

typedef enum E_RWOption
{
    READ_ONLY,
    WRITE_ONLY,
}RWOption;

typedef enum E_FileState
{
    FILE_NOT_OPEN,
    FILE_OPEN,
    FILE_CLOSE,
    FILE_ERROR,
}FileState;

class File
{
    public:
        IReader& ireader;
        File(IReader& iread, std::string &filepath, RWOption option);
        ~File();
        //std::string& filepath();
        void open_file();
        void close_file();
        int  read(char *buf);
        void write(char *buf);

    private:
        //File();
        std::string filepath;
        RWOption    option;
        int         fd;
        FileState   state;

};

#endif
