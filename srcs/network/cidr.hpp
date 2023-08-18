#ifndef CIDR_HPP
#define CIDR_HPP
#include <string>
#include <ip_address.hpp>

class CIDR
{
    public:
        CIDR();
        ~CIDR();
        static CIDR from_string(std::string const &str);
        bool in(IP_Address const & address) const;
    private:
        IP_Address address;
        bool all;
        int subnet;
};

#endif
