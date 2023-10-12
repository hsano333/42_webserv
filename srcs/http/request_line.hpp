#ifndef REQUEST_LINE_HPP
#define REQUEST_LINE_HPP
#include "method.hpp"
#include "uri.hpp"
#include "http_version.hpp"

class RequestLine
{
    public:
        RequestLine();
        ~RequestLine();
        RequestLine(RequestLine const &line);
        RequestLine& operator=(RequestLine const &line);
        static RequestLine from_string(std::string &str);
        void   print_info() const;

        Method      const &method();
        URI         const &uri();
        HttpVersion const &version();
        //std::string &version();
    private:
        Method      method_;
        URI         uri_;
        HttpVersion version_;
};

#endif
