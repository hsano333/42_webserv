#include "get_method.hpp"
#include <string>

using std::string;

GetMethod::GetMethod()
{
}

GetMethod::~GetMethod()
{
}

const std::string GetMethod::_myname = "GET";

const string& GetMethod::get_name() const
{
    return (GetMethod::_myname);
}

void GetMethod::parse()
{
}

void GetMethod::execute()
{
}

bool GetMethod::is_need_body() const
{
    return (false);
}

Method* GetMethod::make_method(std::string &name)
{
    (void)name;
    return (NULL);
}
