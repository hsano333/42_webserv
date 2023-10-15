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

Method::Method(Method const &method)
{
    this->method_ = method.method_;
}

Method& Method::operator=(Method const &method)
{
    this->method_ = method.method_;
    return (*this);
}

bool Method::operator==(Method const &method)
{
    return (this->method_ == method.method_);
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

Method Method::from_string(string const &name)
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

std::string Method::to_string() const
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

e_method Method::to_enum() const
{
    return (this->method_);
}

#ifdef UNIT_TEST
#include "doctest.h"

TEST_CASE("HTTP_METHOD")
{
    CHECK_THROWS_AS(Method::from_string("PUT") ,std::invalid_argument);
    CHECK_THROWS_AS(Method::from_string("GET1") ,std::invalid_argument);
    CHECK_THROWS_AS(Method::from_string("get") ,std::invalid_argument);
    CHECK_THROWS_AS(Method::from_string("SGET") ,std::invalid_argument);
    CHECK_THROWS_AS(Method::from_string("1.2") ,std::invalid_argument);
    CHECK(Method::from_string("GET").to_string() == "GET");
    CHECK(Method::from_string("POST").to_string() == "POST");
    CHECK(Method::from_string("DELETE").to_string() == "DELETE");
}
#endif
