#include "status_code.hpp"

using std::make_pair;
StatusCode::StatusCode()
{
    ;
}

StatusCode::~StatusCode()
{
    ;
}

// int StatusCode::test = 0;
void StatusCode::init()
{
    // StatusCode::test = 1;
    // StatusCode::test = 1;
    //  StatusCode::test = 1;
    //   StatusCode::_status_code_strs.insert(make_pair(200, "OK"));
    StatusCode::_status_code_strs.insert(make_pair(200, "OK"));
    StatusCode::_status_code_strs.insert(make_pair(204, "Not Content"));
    StatusCode::_status_code_strs.insert(make_pair(206, "Partial Content"));
    StatusCode::_status_code_strs.insert(make_pair(301, "Moved Permanently"));
    StatusCode::_status_code_strs.insert(make_pair(302, "Not Found"));
    StatusCode::_status_code_strs.insert(make_pair(303, "See Other"));
    StatusCode::_status_code_strs.insert(make_pair(304, "Not Modified"));
    StatusCode::_status_code_strs.insert(make_pair(307, "Temporary Redirect"));
    StatusCode::_status_code_strs.insert(make_pair(400, "Bad Request"));
    StatusCode::_status_code_strs.insert(make_pair(401, "Bad Unauthorized"));
    StatusCode::_status_code_strs.insert(make_pair(403, "Forbidden"));
    StatusCode::_status_code_strs.insert(make_pair(404, "Not Found"));
    StatusCode::_status_code_strs.insert(make_pair(500, "Internal Server Error"));
    StatusCode::_status_code_strs.insert(make_pair(503, "Service Unavailable"));
}

/*
std::string StatusCode::get_string_of(int status_code)
{
    return (StatusCode::_status_code_strs[status_code]);
}
*/
