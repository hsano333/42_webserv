#include "no_request_line.hpp"

NoRequestLineException::NoRequestLineException()
{
}
NoRequestLineException::~NoRequestLineException() throw()
{
}

const char *NoRequestLineException::what() const throw()
{
    return ("");
}
