#ifndef HEADER_HPP
#define HEADER_HPP

#include "split.hpp"
#include <map>
#include <string>
#include <iostream>
#include <algorithm>

using std::string;
using std::map;
using std::cout;
using std::endl;

#define LocationHeader "Location"
#define CORS_ORIGIN "Access-Control-Allow-Origin"
#define CORS_ORIGIN_VALUE "*"

class Header{
    public:
        Header();
        ~Header();
        Header(Header const &header);
        Header& operator=(Header const &header);
        void print_info() const;
        std::string const &find(const std::string &name) const;
        ssize_t get_content_length() const;
        std::string const &get_content_length_str() const;
        bool is_chunked();
        std::string const &get_host() const;
        static Header from_splited_data(Split &sp, size_t offset);
    private:
        std::map<std::string, std::string > _headers;
        std::string const _not_find;
};
#endif
