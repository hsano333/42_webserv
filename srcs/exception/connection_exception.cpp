#include "connection_exception.hpp"

ConnectionException::ConnectionException()
{
    ;
}

ConnectionException::ConnectionException(std::string const &str) : error(str)
{
    ;
}

ConnectionException::~ConnectionException() throw()
{
    ;
}

const char *ConnectionException::what() const throw()
{
    return (error.c_str());
}
