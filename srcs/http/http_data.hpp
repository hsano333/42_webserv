
#ifndef HTTP_DATA_HPP
#define HTTP_DATA_HPP
#include "status_code.hpp"
#include "webserv_file.hpp"
#include <unistd.h>
#include <map>

class HttpData
{
    public:
        virtual ~HttpData(){};
        virtual WebservFile *get_source_file() = 0;
        virtual int open_source_file() = 0;
        virtual int close_source_file() = 0;
        virtual ssize_t get_data(char** data) = 0;

        //virtual int raw_buf_space();
        //virtual char* get_raw_buf_pointer();
    private:
};

#endif /* RESPONSE_HPP */
