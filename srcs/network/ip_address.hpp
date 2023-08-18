#ifndef IP_ADDRESS_HPP
#define IP_ADDRESS_HPP
#include <string>

class IP_Address
{
    public:
        IP_Address();
        ~IP_Address();
        static IP_Address from_string(std::string const &str);
        //bool in(IP_Address const & cidr);
        std::string to_string();
        unsigned int to_uint() const;
    private:
        unsigned  int address;

};

#endif
