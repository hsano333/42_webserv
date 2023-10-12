
#include "http_exception.hpp"

HttpException::HttpException()
{
    ;
}

HttpException::HttpException(std::string const &str) : error(str)
{
    ;
}

HttpException::~HttpException() throw()
{
    ;
}

const char *HttpException::what() const throw()
{
    return (error.c_str());
}
