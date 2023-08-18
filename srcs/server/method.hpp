#ifndef METHOD_HPP
#define METHOD_HPP
#include <string>

typedef enum E_METHOD
{
    GET,
    POST,
    DELETE,

} e_method;

//class Method;

class Method
{
    public:
        Method();
        ~Method();
        //virtual ~Method() {};
        //virtual void parse() = 0;
        //virtual void execute() = 0;
        //virtual const std::string& get_name() const = 0;
        //virtual bool is_need_body() const = 0;
        static Method from_string(std::string &name);
        std::string const to_string() const;
        //static Method* make_method(std::string &name);
    private:
        e_method method_;

};

#endif
