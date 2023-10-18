#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include "status_code.hpp"
#include "error_page.hpp"

class Response
{
    public:
        Response();
        ~Response();
        Response(Response const &res);
        Response& operator=(Response const &res);
        static Response* from_status_code(StatusCode &code);
        static Response* from_error_page(ErrorPage &page);

        static Response* from_redirect(StatusCode &code, std::string const &filepath);
        static Response* from_file(std::string const &filepath);
        static Response* from_directory(std::string const &filepath);

        void print_info();

    private:
        StatusCode status_code;
};


#endif /* RESPONSE_HPP */
