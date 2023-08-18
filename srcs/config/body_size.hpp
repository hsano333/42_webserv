#ifndef BODY_SIZE_HPP
#define BODY_SIZE_HPP
#include <string>
#include <climits>
#include <unistd.h>

class BodySize
{
    public:
        BodySize();
        ~BodySize();
        static BodySize from_string (std::string const &size);
        static BodySize from_string (std::string &size);
        static BodySize from_number (ssize_t size);
        size_t to_number();
    private:
        size_t size;
        //bool check_over(ssize_t value, ssize_t unit);
        //ssize_t get_unit_size(char c);

};

#endif
