#ifndef HTTP_EXCEPTION_HPP
#define HTTP_EXCEPTION_HPP

#include <exception>
#include <string>

class HttpException: public std::exception
{
    public:
        HttpException();
        HttpException(std::string const &str);
        ~HttpException() throw();
        const char* what() const throw();
    private:
        std::string error;
};
#endif
