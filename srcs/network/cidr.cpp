#include "cidr.hpp"
#include "split.hpp"
#include "global.hpp"
#include "utility.hpp"
#include <vector>

using std::cout;
using std::endl;

CIDR::CIDR() : all(false), subnet(0)
{
;
}

CIDR::~CIDR()
{
;
}

#include <stdio.h>
bool CIDR::in(IP_Address const & address) const
{
    (void)address;
    //(void)address;
    unsigned int target_addr = address.to_uint();
    (void)target_addr ;
    unsigned int addr = this->address.to_uint();
    unsigned int subnet = this->subnet;
    //unsigned int min_address = address >> (32-subnet);
    //min_address = min_address << (32-subnet);

    unsigned int bit = 0;
    for(unsigned int i=0;i<subnet;i++){
        bit = bit << 1;
        bit += 1;
    }
    bit = bit << (32-subnet);

    unsigned int min_address = addr & bit;
    unsigned int max_address = (min_address | (bit ^ 0xffffffff));
    //cout << "min_address:" << min_address << endl;
    //cout << "max_address:" << max_address << endl;

    if(min_address <= target_addr && target_addr <= max_address)
    {
        return (true);
    }
    return (false);
}



CIDR CIDR::from_string(std::string const &str)
{
    CIDR object;
    Split sp(str, "/");

    object.all = false;
    object.subnet = 0;
    if(str == "all"){
        object.all = true;
        object.subnet= 0;
        return object;
    }

    if(sp.size() > 2){
        ERROR("CIDR class: invalid argument:" + str);
        throw std::invalid_argument("CIDR class: invalid argument");
    }
    if(sp.size() == 2){
        try{
            unsigned int tmp_subnet = Utility::to_uint(sp[1]);
            if(tmp_subnet <= 32u){
                object.subnet = Utility::to_int(sp[1]);
            }else{
                ERROR("CIDR class: invalid argument:" + str);
                throw std::invalid_argument("CIDR class: invalid argument");
            }
        }catch (std::exception &e){
            ERROR("CIDR class: invalid argument:" + str);
            throw std::invalid_argument("CIDR class: invalid argument");
        }
    }else{
        if(str.find("/") != std::string::npos){
            ERROR("CIDR class: invalid argument:" + str);
            throw std::invalid_argument("CIDR class: invalid argument");
        }
    }

    object.address = IP_Address::from_string(sp[0]);
    return (object);
}

#ifdef UNIT_TEST
#include "doctest.h"

TEST_CASE("CIDR")
{
    CHECK_THROWS_AS(CIDR::from_string("256.255.255.255") ,std::invalid_argument);
    CHECK_THROWS_AS(CIDR::from_string("255.255.255.255/") ,std::invalid_argument);
    CHECK_THROWS_AS(CIDR::from_string("255.255.255.255/33") ,std::invalid_argument);
    CHECK_THROWS_AS(CIDR::from_string("255.255.255.255/32a") ,std::invalid_argument);
    CIDR ip1 = CIDR::from_string("255.255.255.255/31");
    IP_Address addr1 = IP_Address::from_string("255.255.255.255");
    IP_Address addr2 = IP_Address::from_string("255.255.255.254");
    IP_Address addr3 = IP_Address::from_string("255.255.255.253");
    REQUIRE(ip1.in(addr1) == true);
    REQUIRE(ip1.in(addr2) == true);
    REQUIRE(ip1.in(addr3) == false);


    CIDR ip2 = CIDR::from_string("192.168.0.1/16");
    IP_Address addr4 = IP_Address::from_string("192.168.0.0");
    IP_Address addr5 = IP_Address::from_string("192.168.255.255");
    IP_Address addr6 = IP_Address::from_string("192.169.0.0");
    IP_Address addr7 = IP_Address::from_string("192.167.255.255");
    REQUIRE(ip2.in(addr4) == true);
    REQUIRE(ip2.in(addr5) == true);
    REQUIRE(ip2.in(addr6) == false);
    REQUIRE(ip2.in(addr7) == false);


    CIDR ip3 = CIDR::from_string("192.168.0.1/0");
    IP_Address addr8 = IP_Address::from_string("0.0.0.0");
    IP_Address addr9 = IP_Address::from_string("255.255.255.255");
    REQUIRE(ip3.in(addr8) == true);
    REQUIRE(ip3.in(addr9) == true);

    //IP_Address ip2 = IP_Address::from_string("0.1.2.3");
    //REQUIRE(ip2.to_string() == "0.1.2.3");
}

#endif
