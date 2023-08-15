#include "port.hpp"
#include "global.hpp"
#include "utility.hpp"

Port::Port()
{
    ;
}

Port::~Port()
{
    ;
}

Port Port::from_string(std::string &port)
{
    Port object;
    int number = Utility::to_int(port);
    if(65535 >= number &&  number > 0){
        object.port = number;
        return object;
    }
    WARNING("Port number Invalid argument:" + number);
    throw std::invalid_argument("Port number:Invalid Argument");
}

Port Port::from_int(int number)
{
    Port object;
    if(65535 >= number &&  number > 0){
        object.port = number;
        return object;
    }
    return object;
}

std::string Port::to_string() const
{
    return (Utility::to_string(this->port));
}
std::string Port::to_string()
{
    return (Utility::to_string(this->port));
}

const char* Port::c_str() const
{
    return (Utility::to_string(this->port)).c_str();
}
const char* Port::c_str()
{
    return (Utility::to_string(this->port)).c_str();
}


bool Port::operator==(Port const & port) const
{
    return (this->port == port.port);
}

bool Port::operator==(Port const & port)
{
    return (this->port == port.port);
}

bool Port::operator<(Port const & port)
{
    return (this->port < port.port);
}

bool Port::operator<(Port const & port) const
{
    return (this->port < port.port);
}


