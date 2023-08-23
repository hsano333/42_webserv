#include "port.hpp"
#include "global.hpp"
#include "utility.hpp"

Port::Port() : port(0)
{
    ;
}

Port::Port(Port const &port)
{
    this->port = port.port;
}

Port::~Port()
{
    ;
}
Port& Port::operator=(Port const &port)
{
    this->port = port.port;
    return (*this);
}

Port Port::from_string(std::string const &port)
{
    Port object;
    unsigned int number = Utility::to_uint(port);
    if(65535 >= number && number > 0){
        object.port = number;
        return (object);
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
    WARNING("Port number Invalid argument:" + number);
    throw std::invalid_argument("Port number:Invalid Argument");
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

bool Port::is_valid()
{
    if(this->port == 0){
        return false;
    }
    return true;
}



#ifdef UNIT_TEST
#include "doctest.h"
TEST_CASE("Port class")
{
    //Port port = Port::from_int();
    CHECK_THROWS_AS(Port::from_int(0) ,std::invalid_argument);
    CHECK_THROWS_AS(Port::from_int(-1) ,std::invalid_argument);
    CHECK_THROWS_AS(Port::from_int(65536) ,std::invalid_argument);
    CHECK_THROWS_AS(Port::from_string("65536") ,std::invalid_argument);
    CHECK_THROWS_AS(Port::from_string("0") ,std::invalid_argument);
    CHECK_THROWS_AS(Port::from_string("-1") ,std::invalid_argument);
    CHECK_THROWS_AS(Port::from_string("1.0") ,std::invalid_argument);
    CHECK_THROWS_AS(Port::from_string("1a") ,std::invalid_argument);

    CHECK(Port::from_string("1").to_string() == "1");
    CHECK(Port::from_int(1).to_string() == "1");
    CHECK(Port::from_string("65535").to_string() == "65535");
    CHECK(Port::from_int(65535).to_string() == "65535");
}

#endif
