#ifndef CGI_FILE_HPP
#define CGI_FILE_HPP

#include <string>
#include "normal_file.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include "ireader.hpp"
#include "iwriter.hpp"
#include "file_discriptor.hpp"
#include "file.hpp"


/*
typedef enum E_FileState
{
    FILE_NOT_OPEN,
    FILE_OPEN,
    FILE_CLOSE,
    FILE_ERROR,
}FileState;
*/

class CGIFile : public File
{
    public:
        ~CGIFile();
        //static CGIFile* from_filepath(IReader* iread, std::string &filepath, RWOption option);
        static CGIFile* from_fd(FileDiscriptor fd, RWOption option);
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
        CGIFile();
        //CGIFile(IReader* iread, std::string const &filepath, RWOption option);
        CGIFile(IReader* iread, FileDiscriptor fd);
        CGIFile(IWriter* iwriter, FileDiscriptor fd);
        IReader *ireader;
        IWriter *iwriter;
        //std::string filepath;
        //RWOption    option;
        //std::ios_base::openmode option;
        FileDiscriptor fd;
        FileState   state;
        std::fstream iofs;
        std::vector<char> tmp_buf;

};

#endif
