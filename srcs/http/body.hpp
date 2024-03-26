#ifndef BODY_HPP
#define BODY_HPP

#include <cstdio>
#include <string>

class Body
{
    public:
        Body();
        bool is_text;
        bool is_chunk;
        size_t content_length;
        //const char* boundary;
        void set_boundary(const char *boundary);
        std::string &boundary();
    private:
        std::string boundary_;

};

#endif
