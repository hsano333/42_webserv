#ifndef BODY_SIZE_HPP
#define BODY_SIZE_HPP
#include <string>
#include <climits>
#include <unistd.h>

class BodySize
{
    public:
        BodySize();
        BodySize(size_t size);
        ~BodySize();
        static BodySize from_string(std::string const &size);
        static BodySize from_string(std::string &size);
        static BodySize from_number(ssize_t size);
        static BodySize from_number_string(std::string &size);
        size_t to_number() const;
    private:
        size_t size;
};

#endif
