#ifndef HEADER_HPP
#define HEADER_HPP

#include <map>
#include <string>
#include <iostream>
#include <algorithm>

using std::string;
using std::map;
using std::cout;
using std::endl;

class Header{
    public:
        Header();
        ~Header();
        void load_data(char *buf);
        void print() const;
        bool get_end();
        std::string& find(const std::string &name);
        size_t get_content_length();
        void check_header_related_body();
    private:
        //int _fd;
        std::map<std::string, std::string > _headers;
        std::string _not_find;
        bool _err;
        bool _err_code;
        size_t _content_length;
        bool _chunked;
        //RawRequestReader _buf;
        //const size_t limit_size = 2048;
        //std::map<std::string, std::string> _header;
        //std::string _rest;
        bool _end;
};
#endif
