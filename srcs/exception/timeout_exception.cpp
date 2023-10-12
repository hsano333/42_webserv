#include "timeout_exception.hpp"

TimeoutException::TimeoutException()
{
    ;
}

TimeoutException::TimeoutException(std::string const &str) : error(str)
{
    ;
}

TimeoutException::~TimeoutException() throw()
{
    ;
}

const char *TimeoutException::what() const throw()
{
    return (error.c_str());
}
