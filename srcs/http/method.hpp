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

class Method
{
    public:
        Method();
        ~Method();
        Method(Method const &method);
        Method& operator=(Method const &method);
        bool operator==(Method const &method);
        bool operator==(E_METHOD const method) const;

        static Method from_string(std::string const &name);
        std::string to_string() const;
        E_METHOD to_enum() const;
    private:
        E_METHOD method_;
};

#endif
