#include "request_line.hpp"
#include "split.hpp"
#include "global.hpp"
#include "method.hpp"
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

RequestLine::RequestLine()
{
    ;
}

RequestLine::~RequestLine()
{
    ;
}

RequestLine::RequestLine(RequestLine const &line)
{
    this->method_ = line.method_;
    this->uri_ = line.uri_;
    this->version_ = line.version_;
}

RequestLine& RequestLine::operator=(RequestLine const &line)
{
    this->method_ = line.method_;
    this->uri_ = line.uri_;
    this->version_ = line.version_;
    return (*this);
}

RequestLine RequestLine::from_string(std::string const &str)
{

    cout << "RequestLine::from_string: " << str << endl;
    Split sp(str, " ");
    if(sp.size() != 3){
        ERROR("Request line doesn't divided to three str=" + str);
        throw std::invalid_argument("Request line doesn't divided to three");
    }
    RequestLine req_line;

    //cout << "test sp1:"
    req_line.method_ = Method::from_string(sp[0]);
    req_line.uri_ = URI::from_string(sp[1]);
    req_line.version_ = HttpVersion::from_string(sp[2]);
    return (req_line);
}

void RequestLine::print_info() const
{
    cout << "|-- Print RequestLine --|" << endl;
    cout << this->method_.to_string() << endl;
    cout << this->version_.to_string() << endl;
    this->uri_.print_info();

    cout << "|--------------------------|" << endl;

}
Method const &RequestLine::method() const
{
    return (this->method_);
}

URI const &RequestLine::uri() const
{
    return (uri_);
}

HttpVersion const &RequestLine::version() const
{
    return (version_);
}
