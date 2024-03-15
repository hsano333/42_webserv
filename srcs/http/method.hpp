#ifndef METHOD_HPP
#define METHOD_HPP
#include <string>

typedef enum E_METHOD
{
    GET,
    POST,
    DELETE,

    HEAD,
    PUT,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
    NONE,
} E_METHOD;

//class Method;

class Method
{
    public:
        Method();
        ~Method();
        Method(Method const &method);
        Method& operator=(Method const &method);
        bool operator==(Method const &method);
        bool operator==(E_METHOD const method) const;

        //virtual ~Method() {};
        //virtual void parse() = 0;
        //virtual void execute() = 0;
        //virtual const std::string& get_name() const = 0;
        //virtual bool is_need_body() const = 0;
        static Method from_string(std::string const &name);
        std::string to_string() const;
        E_METHOD to_enum() const;
        //static Method* make_method(std::string &name);
    private:
        E_METHOD method_;
};

#endif
