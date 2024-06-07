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
        static RequestLine from_string(std::string const &str);
        void   print_info() const;

        Method      const &method() const;
        URI         const &uri() const;
        HttpVersion const &version() const;
    private:
        Method      method_;
        URI         uri_;
        HttpVersion version_;
};

#endif
