#include "status_code.hpp"
#include "utility.hpp"
#include "global.hpp"


StatusCode::StatusCode()
{
    ;
}

StatusCode::~StatusCode()
{
    ;
}

const char* StatusCode::message() const
{
    switch (this->code)
    {
        case 200:
            return MESSAGE200;
        case 204:
            return MESSAGE204;
        case 206:
            return MESSAGE206;
        case 301:
            return MESSAGE301;
        case 302:
            return MESSAGE302;
        case 303:
            return MESSAGE303;
        case 304:
            return MESSAGE304;
        case 307:
            return MESSAGE307;
        case 400:
            return MESSAGE400;
        case 401:
            return MESSAGE401;
        case 403:
            return MESSAGE403;
        case 404:
            return MESSAGE404;
        case 500:
            return MESSAGE500;
        case 503:
            return MESSAGE503;
        default:
            return MESSAGEUnknown;
    }
}

StatusCode StatusCode::from_int(int code)
{
    StatusCode tmp_code;
    if(100 <= code && code < 600){
        tmp_code.code = code;
    }else{
        ERROR("StatusCode Invalid arguments:" + Utility::to_string(code));
        throw std::runtime_error("StatusCode Invalid arguments");
    }
    return (tmp_code);
}

StatusCode StatusCode::from_string(std::string const &str)
{
    int code = Utility::to_int(str);
    return StatusCode::from_int(code);
}

int StatusCode::to_int() const
{
    return (this->code);
}

std::string StatusCode::to_string() const
{
    return (Utility::to_string(code));
}

bool StatusCode::operator==(StatusCode const &code)
{
    return (this->code == code.code);
}
bool StatusCode::operator==(StatusCode const &code) const
{
    return (this->code == code.code);
}

bool StatusCode::operator<(StatusCode const &code)
{
    return (this->code < code.code);
}

bool StatusCode::operator<(StatusCode const &code) const
{
    return (this->code < code.code);
}




bool StatusCode::operator==(int code) const
{
    return (this->code == code);
}

bool StatusCode::operator<(int code) const
{
    return (this->code < code);
}

bool StatusCode::operator<=(int code) const
{
    return (this->code <= code);
}

bool StatusCode::operator>(int code) const
{
    return (this->code > code);
}

bool StatusCode::operator>=(int code) const
{
    return (this->code >= code);
}
