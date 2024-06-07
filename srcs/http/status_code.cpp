#include "status_code.hpp"
#include "utility.hpp"
#include "global.hpp"

using std::make_pair;


StatusCode::StatusCode(): code(0)
{
    ;
}

StatusCode::~StatusCode()
{
    ;
}

StatusCode::StatusCode(StatusCode const &code)
{
    this->code = code.code;
}

StatusCode &StatusCode::operator=(StatusCode const &code)
{
    if(this == &code){
        return (*this);
    }
    this->code = code.code;
    return (*this);
}

const char* StatusCode::message() const
{
    std::map<int, std::string>::iterator message = StatusCode::code_message_list.find(this->code);
    if(message == StatusCode::code_message_list.end()){
        return (MESSAGEUnknown);
    }
    return (message->second.c_str());


    switch (this->code)
    {
        case 200:
            return MESSAGE200;
        case 204:
            return MESSAGE204;
        case 206:
            return MESSAGE206;
        case 207:
            return MESSAGE207;
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
        case 405:
            return MESSAGE405;
        case 408:
            return MESSAGE408;
        case 413:
            return MESSAGE413;
        case 500:
            return MESSAGE500;
        case 503:
            return MESSAGE503;
        case 504:
            return MESSAGE504;
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


std::map<int, std::string> StatusCode::code_message_list;
void StatusCode::init_http_status_message()
{
    //map<string, string> status_codes;
    // 1xx
    StatusCode::code_message_list.insert(make_pair(100, "Continue"));
    StatusCode::code_message_list.insert(make_pair(101, "Switching Protocols"));
    StatusCode::code_message_list.insert(make_pair(102, "Processing"));
    // 2xx
    StatusCode::code_message_list.insert(make_pair(200, "OK"));
    StatusCode::code_message_list.insert(make_pair(201, "Created"));
    StatusCode::code_message_list.insert(make_pair(202, "Accepted"));
    StatusCode::code_message_list.insert(make_pair(203, "Non-Authoritative Information"));
    StatusCode::code_message_list.insert(make_pair(204, "No Content"));
    StatusCode::code_message_list.insert(make_pair(205, "Reset Content"));
    StatusCode::code_message_list.insert(make_pair(206, "Partial Content"));
    StatusCode::code_message_list.insert(make_pair(207, "Multi-Status"));
    StatusCode::code_message_list.insert(make_pair(208, "Already Reported"));
    StatusCode::code_message_list.insert(make_pair(226, "IM Used"));
    // 3xx
    StatusCode::code_message_list.insert(make_pair(300, "Multiple Choices"));
    StatusCode::code_message_list.insert(make_pair(301, "Moved Permanently"));
    StatusCode::code_message_list.insert(make_pair(302, "Found"));
    StatusCode::code_message_list.insert(make_pair(303, "See Other"));
    StatusCode::code_message_list.insert(make_pair(304, "Not Modified"));
    StatusCode::code_message_list.insert(make_pair(305, "Use Proxy"));
    StatusCode::code_message_list.insert(make_pair(307, "Temporary Redirect"));
    StatusCode::code_message_list.insert(make_pair(308, "Permanent Redirect"));
    // 4xx
    StatusCode::code_message_list.insert(make_pair(400, "Bad Request"));
    StatusCode::code_message_list.insert(make_pair(401, "Unauthorized"));
    StatusCode::code_message_list.insert(make_pair(402, "Payment Required"));
    StatusCode::code_message_list.insert(make_pair(403, "Forbidden"));
    StatusCode::code_message_list.insert(make_pair(404, "Not Found"));
    StatusCode::code_message_list.insert(make_pair(405, "Method Not Allowed"));
    StatusCode::code_message_list.insert(make_pair(406, "Not Acceptable"));
    StatusCode::code_message_list.insert(make_pair(407, "Proxy Authentication Required"));
    StatusCode::code_message_list.insert(make_pair(408, "Request Timeout"));
    StatusCode::code_message_list.insert(make_pair(409, "Conflict"));
    StatusCode::code_message_list.insert(make_pair(410, "Gone"));
    StatusCode::code_message_list.insert(make_pair(411, "Length Required"));
    StatusCode::code_message_list.insert(make_pair(412, "Precondition Failed"));
    StatusCode::code_message_list.insert(make_pair(413, "Payload Too Large"));
    StatusCode::code_message_list.insert(make_pair(414, "URI Too Long"));
    StatusCode::code_message_list.insert(make_pair(415, "Unsupported Media Type"));
    StatusCode::code_message_list.insert(make_pair(416, "Range Not Satisfiable"));
    StatusCode::code_message_list.insert(make_pair(417, "Expectation Failed"));
    StatusCode::code_message_list.insert(make_pair(418, "I'm a teapot"));
    StatusCode::code_message_list.insert(make_pair(421, "Misdirected Request"));
    StatusCode::code_message_list.insert(make_pair(422, "Unprocessable Entity"));
    StatusCode::code_message_list.insert(make_pair(423, "Locked"));
    StatusCode::code_message_list.insert(make_pair(424, "Failed Dependency"));
    StatusCode::code_message_list.insert(make_pair(425, "Too Early"));
    StatusCode::code_message_list.insert(make_pair(426, "Upgrade Required"));
    StatusCode::code_message_list.insert(make_pair(428, "Precondition Required"));
    StatusCode::code_message_list.insert(make_pair(429, "Too Many Requests"));
    StatusCode::code_message_list.insert(make_pair(431, "Request Header Fields Too Large"));
    StatusCode::code_message_list.insert(make_pair(451, "Unavailable For Legal Reasons"));
    // 5xx
    StatusCode::code_message_list.insert(make_pair(500, "Internal Server Error"));
    StatusCode::code_message_list.insert(make_pair(501, "Not Implemented"));
    StatusCode::code_message_list.insert(make_pair(502, "Bad Gateway"));
    StatusCode::code_message_list.insert(make_pair(503, "Service Unavailable"));
    StatusCode::code_message_list.insert(make_pair(504, "Gateway Timeout"));
    StatusCode::code_message_list.insert(make_pair(505, "HTTP Version Not Supported"));
    StatusCode::code_message_list.insert(make_pair(506, "Variant Also Negotiates"));
    StatusCode::code_message_list.insert(make_pair(507, "Insufficient Storage"));
    StatusCode::code_message_list.insert(make_pair(508, "Loop Detected"));
    StatusCode::code_message_list.insert(make_pair(510, "Not Extended"));
    StatusCode::code_message_list.insert(make_pair(511, "Network Authentication Required"));
}


