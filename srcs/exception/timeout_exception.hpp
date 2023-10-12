#ifndef TIMEOUT_EXCEPTION_HPP
#define TIMEOUT_EXCEPTION_HPP

#include <exception>
#include <string>

class TimeoutException: public std::exception
{
    public:
        TimeoutException();
        TimeoutException(std::string const &str);
        ~TimeoutException() throw();
        const char* what() const throw();
    private:
        std::string error;
};
#endif
