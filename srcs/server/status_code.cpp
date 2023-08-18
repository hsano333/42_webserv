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

StatusCode StatusCode::from_string(std::string &str)
{
    int code = Utility::to_int(str);
    return StatusCode::from_int(code);
}

int StatusCode::to_int() const
{
    return (this->code);
}

std::string StatusCode::to_string()
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
