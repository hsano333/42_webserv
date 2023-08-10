#ifndef FILEPATH_HPP
#define FILEPATH_HPP

#include <string>

typedef enum E_RWOption
{
    READ_ONLY,
    WRITE_ONLY,

}RWOption;

class Filepath
{
    public:
        Filepath(std::string filepath, RWOption option);
        ~Filepath();
        std::string to_str();
    private:
        std::string filepath;
        RWOption    option;
};

#endif
