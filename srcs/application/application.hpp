#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#include "response.hpp"

class Application
{
    public:
        //Application();
        virtual ~Application(){};
        virtual bool execute() = 0;
        virtual void check_permission() = 0;
        virtual bool is_cgi() const = 0;
        virtual Response* make_response() = 0;
        //virtual void set_path_info(std::string const &path_info) = 0;
        //virtual std::string &get_path_info() = 0;
    private:


};
#endif
