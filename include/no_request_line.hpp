#ifndef NO_REQUEST_LINE_HPP
#define NO_REQUEST_LINE_HPP
#include <exception>
#include <string>

class NoRequestLineException: public std::exception
{
    public:
        NoRequestLineException();
        ~NoRequestLineException() throw();
        const char* what() const throw();
    private:
};
#endif
