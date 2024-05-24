#ifndef BUFFER_CONTROLLER_HPP
#define BUFFER_CONTROLLER_HPP
#include <dirent.h>
#include <vector>

class BufferController
{
    public:
        BufferController();
        ~BufferController();
        void copy(std::vector<char> &buf);
        int save(char *data, size_t size);
        int save(char const *data, size_t size);
        size_t append(char const *data, size_t size);
        int retrieve(char **data, size_t max_size);
        void ref(char **data, size_t size);
        void clear();
        
        size_t size() const;
    private:
        std::vector<char> buf;

};

#endif
