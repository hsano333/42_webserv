#ifndef CONNECTION_EXCEPTION_HPP
#define CONNECTION_EXCEPTION_HPP

#include <exception>
#include <string>

class ConnectionException: public std::exception
{
    public:
        ConnectionException();
        ConnectionException(std::string const &str);
        ~ConnectionException() throw();
        const char* what() const throw();
    private:
        std::string error;
};
#endif
