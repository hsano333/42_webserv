#ifndef MYFILE_HPP
#define MYFILE_HPP
#include <unistd.h>
#include <string>


typedef enum E_FileState
{
    FILE_NOT_OPEN,
    FILE_OPEN,
    FILE_READING,
    FILE_WRITING,
    FILE_COMPLETED_READ,
    FILE_CLOSE,
    FILE_ERROR,
}FileState;

class File
{
    public:
        virtual ~File(){};
        virtual int open() = 0;
        virtual int read(char **data, size_t size) = 0;
        virtual int write(char **data, size_t size) = 0;
        virtual int save(char *data, size_t size) = 0;
        virtual int close() = 0;

        virtual bool can_read() = 0;
        virtual int remove() = 0;
        //virtual bool is_chunk() = 0;
        //virtual size_t size() = 0;
        virtual std::string const &path() = 0;
    private:

};

#endif
