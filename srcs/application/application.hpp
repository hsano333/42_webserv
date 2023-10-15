#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#include "response.hpp"

class Application
{
    public:
        //Application();
        virtual ~Application(){};
        virtual void execute() = 0;
        virtual Response* make_response() = 0;
    private:


};
#endif
