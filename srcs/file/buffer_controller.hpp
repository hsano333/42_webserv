#ifndef BUFFER_CONTROLLER_HPP
#define BUFFER_CONTROLLER_HPP
#include <dirent.h>
#include <vector>

class BufferController
{
    public:
        BufferController();
        ~BufferController();
        int save(char *data, size_t size);
        int retrieve(char **data, size_t max_size);
    private:
        std::vector<char> buf;

};

#endif
