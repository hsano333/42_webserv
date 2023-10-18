#ifndef IP_ADDRESS_HPP
#define IP_ADDRESS_HPP
#include <string>

class IP_Address
{
    public:
        IP_Address();
        ~IP_Address();
        bool operator==(IP_Address const ip);
        static IP_Address from_int(int i1, int i2, int i3, int i4);
        static IP_Address from_string(std::string const &str);
        static IP_Address from_name(std::string const &name);
        static IP_Address from_string_or_name(std::string const &name);
        //bool in(IP_Address const & cidr);
        std::string to_string();
        unsigned int to_uint() const;
    private:
        unsigned  int address;

};

#endif
