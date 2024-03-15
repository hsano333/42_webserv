#ifndef HEADER_HPP
#define HEADER_HPP

#include "split.hpp"
#include "http_exception.hpp"
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
        //using iterator = std::map<std::string, std::string >;
        Header();
        ~Header();
        Header(Header const &header);
        Header& operator=(Header const &header);
        size_t size() const;
        void insert(std::string const &key, std::string const &value);
        void erase(std::string const &key);
        //void insert(std::string const &key, std::string const &value);

        void print_info() const;
        std::string const &not_find() const;
        std::string const &find(const std::string &name) const;
        std::string get_content_type() const;
        ssize_t get_content_length() const;
        std::string const &get_content_length_str() const;
        bool is_chunked() const;
        std::string const &get_host() const;
        static Header from_splited_data(Split &sp, size_t offset);

        //std::pair<string, string> const &operator[](size_t i) const;
        std::map<std::string, std::string >::iterator begin();
        std::map<std::string, std::string >::iterator end();
        std::map<std::string, std::string >::const_iterator cbegin() const;
        std::map<std::string, std::string >::const_iterator cend() const;

        //typedef typename Iter::iterator iterator;
        //typedef typename Iter::const_iterator const_iterator;
    private:
        std::map<std::string, std::string > _headers;
        std::string const _not_find;
};
#endif
