#ifndef GET_METHOD_HPP
#define GET_METHOD_HPP
#include "method.hpp"



class GetMethod: virtual public Method
{
    public:
        GetMethod();
        ~GetMethod();
        void parse();
        void execute();
        const std::string& get_name() const;
        bool is_need_body() const;
        static Method* make_method(std::string &name);
        const static std::string _myname;
    private:

};

#endif
