#include "method.hpp"
#include "get_method.hpp"
#include <string>

using std::string;

/*
Method::Method() : test("get")
{
    ;
}
Method::~Method()
{
    ;
}
*/

/*
const std::string& Method::get_name() const
{
    return (_myname);
}
*/

Method* Method::make_method(string &name)
//Method* make_method(string &name)
{
    if (name == GetMethod::_myname){
        return (new GetMethod());
    }else if (name == GetMethod::_myname){
        return (new GetMethod());
    }else if (name == GetMethod::_myname){
        return (new GetMethod());
    }
    return (NULL);
}

