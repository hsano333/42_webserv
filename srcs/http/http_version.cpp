#include "http_version.hpp"
#include "http_exception.hpp"
#include "global.hpp"
#include "split.hpp"

HttpVersion::HttpVersion()
{
;
}

HttpVersion::~HttpVersion()
{
;
}

HttpVersion::HttpVersion(HttpVersion const &http_version)
{
    this->version = http_version.version;
}

HttpVersion& HttpVersion::operator=(HttpVersion const &http_version)
{
    this->version = http_version.version;
    return (*this);
}

HttpVersion HttpVersion::from_string(std::string const &str)
{
    DEBUG("HttpVersion::from_string: str=" + str);
    HttpVersion version;

    size_t pos = str.find("HTTP/");
    if(pos == std::string::npos || pos != 0){
        ERROR("HttpVersion: Invalid Argument: don't find \"HTTP/\"" + str);
        throw std::invalid_argument("HttpVersion: Invalid Argument");
    }
    Split sp(str, "/");
    if(sp.size() != 2){
        ERROR("HttpVersion: Invalid Argument: don't find \"HTTP/\"" + str);
        throw std::invalid_argument("HttpVersion: Invalid Argument");

    }
    if(sp[1].size() != 3){
        ERROR("HttpVersion: Invalid Argument: don't find \"HTTP/\"" + str);
        throw std::invalid_argument("HttpVersion: Invalid Argument");
    }

    if(sp[1] == "0.9" || sp[1] == "1.0" || sp[1] == "1.1"){
        version.version = str;
        return (version);
    }
    ERROR("HttpVersion: Unsupported Http Version: " + str);
    throw HttpException("505");
}

std::string HttpVersion::to_string() const
{
    return (version);
}

#ifdef UNIT_TEST
#include "doctest.h"

TEST_CASE("HTTP_VERSION")
{
    CHECK_THROWS_AS(HttpVersion::from_string("HTTP/0.8") ,HttpException);
    CHECK_THROWS_AS(HttpVersion::from_string("HTTP/2.0") ,HttpException);
    CHECK_THROWS_AS(HttpVersion::from_string("HTTP/1.2") ,HttpException);
    CHECK_THROWS_AS(HttpVersion::from_string("1.1") ,std::invalid_argument);
    CHECK_THROWS_AS(HttpVersion::from_string("HTTP/1.1a") ,std::invalid_argument);
    CHECK_THROWS_AS(HttpVersion::from_string("HTTPS/1.1") ,std::invalid_argument);
    CHECK_THROWS_AS(HttpVersion::from_string("SHTTP/1.1") ,std::invalid_argument);

    //IP_Address addr09 = HttpVersion::from_string("HTTP/0.9");
    //IP_Address addr10 = HttpVersion::from_string("HTTP/1.0");
    //IP_Address addr11 = HttpVersion::from_string("HTTP/1.1");

    CHECK(HttpVersion::from_string("HTTP/0.9").to_string() == "HTTP/0.9");
    CHECK(HttpVersion::from_string("HTTP/1.0").to_string() == "HTTP/1.0");
    CHECK(HttpVersion::from_string("HTTP/1.1").to_string() == "HTTP/1.1");
}
#endif
