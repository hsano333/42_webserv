#include "method.hpp"
#include "get_method.hpp"
#include "global.hpp"
#include <string>

using std::string;

Method::Method()
{
    ;
}
Method::~Method()
{
    ;
}

/*
const std::string& Method::get_name() const
{
    return (_myname);
}
*/

/*
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
*/

Method Method::from_string(string &name)
{
    Method object;
    if (name == "GET"){
        object.method_ = GET;
        return (object);
    }else if (name == "POST"){
        object.method_ = POST;
        return (object);
    }else if (name == "DELETE"){
        object.method_ = DELETE;
        return (object);
    }
    ERROR("Method::from_string invalid argument:" + name);
    throw std::invalid_argument("Method::from_string invalid argument");
}

std::string const Method::to_string() const
{
    if (method_ == GET){
        return ("GET");
    }else if (method_ == POST){
        return ("POST");
    }else if (method_ == DELETE){
        return ("DELETE");
    }
    return ("");
}


