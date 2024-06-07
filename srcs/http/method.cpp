#include "method.hpp"
#include "http_exception.hpp"
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

bool Method::operator==(E_METHOD const method) const
{
    return (this->method_ == method);
}


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
    }else if (name == "HEAD"){
        object.method_ = HEAD;
        return (object);
    }else if (name == "PUT"){
        object.method_ = PUT;
        return (object);
    }else if (name == "CONNECT"){
        object.method_ = CONNECT;
        return (object);
    }else if (name == "OPTIONS"){
        object.method_ = OPTIONS;
        return (object);
    }else if (name == "TRACE"){
        object.method_ = TRACE;
        return (object);
    }else if (name == "PATCH"){
        object.method_ = PATCH;
        return (object);
    }else if (name == "NONE"){
        object.method_ = NONE;
        return (object);
    }
    throw HttpException("501");
    ERROR("Method::from_string invalid argument:" + name);
    throw HttpException("Method::from_string invalid argument");
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

E_METHOD Method::to_enum() const
{
    return (this->method_);
}

#ifdef UNIT_TEST
#include "doctest.h"

TEST_CASE("HTTP_METHOD")
{
    CHECK_THROWS_AS(Method::from_string("GET1") ,HttpException);
    CHECK_THROWS_AS(Method::from_string("get") ,HttpException);
    CHECK_THROWS_AS(Method::from_string("SGET") ,HttpException);
    CHECK_THROWS_AS(Method::from_string("1.2") ,HttpException);
    CHECK(Method::from_string("GET").to_string() == "GET");
    CHECK(Method::from_string("POST").to_string() == "POST");
    CHECK(Method::from_string("DELETE").to_string() == "DELETE");
    CHECK(Method::from_string("PUT").to_string() == "");
}
#endif
