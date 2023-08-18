#include "ip_address.hpp"
#include "split.hpp"
#include "global.hpp"
#include "utility.hpp"
#include <vector>

IP_Address::IP_Address() 
{
;
}

IP_Address::~IP_Address()
{
;
}

unsigned int IP_Address::to_uint() const
{
    return (address);
}

IP_Address IP_Address::from_string(std::string const &str)
{
    IP_Address object;
    Split number(str, ".");
    if(number.size() != 4){
        ERROR("IP_Address class: invalid argument:" + str);
        throw std::invalid_argument("IP_Address class: invalid argument");
    }
    size_t i = 0;
    int dot_cnt = 0;
    while(str[i]){
        if(str[i] == '.'){
            dot_cnt++;
        }
        i++;
    }
    if(dot_cnt != 3){
        ERROR("IP_Address class: invalid argument:" + str);
        throw std::invalid_argument("IP_Address class: invalid argument");
    }

    std::vector<unsigned short> tmp_vec(4, 0);
    for(size_t i=0;i<4;i++)
    {
        try{
            unsigned int tmp = Utility::to_uint(number[i]);
            if( tmp <= 255u){
                tmp_vec[i] = tmp;
            }else{
                ERROR("IP_Address class: invalid argument:" + number[i] + " in " + str);
                throw std::invalid_argument("IP_Address class: invalid argument");
            }
        }catch (std::exception &e){
            ERROR("IP_Address class: invalid argument:" + str);
            throw std::invalid_argument("IP_Address class: invalid argument");
        }

    }
    object.address = (tmp_vec[0] << 24) | (tmp_vec[1] << 16) | (tmp_vec[2] << 8) | tmp_vec[3];
    return (object);
}


std::string IP_Address::to_string()
{
    unsigned int add0 = 0xff000000;
    unsigned int add1 = 0x00ff0000;
    unsigned int add2 = 0x0000ff00;
    unsigned int add3 = 0x000000ff;

    std::string r_str;
    r_str = Utility::to_string((add0 & this->address) >> 24);
    r_str += "." + Utility::to_string((add1 & this->address) >> 16);
    r_str += "." + Utility::to_string((add2 & this->address) >> 8);
    r_str += "." + Utility::to_string((add3 & this->address) );
    return (r_str);

}



#ifdef UNIT_TEST
#include "doctest.h"

TEST_CASE("IP_Address")
{
    CHECK_THROWS_AS(IP_Address::from_string("256.255.255.255") ,std::invalid_argument);
    CHECK_THROWS_AS(IP_Address::from_string("255.255a.255.255") ,std::invalid_argument);
    CHECK_THROWS_AS(IP_Address::from_string("255.255.-255.255") ,std::invalid_argument);
    CHECK_THROWS_AS(IP_Address::from_string("255.255.255.255 ") ,std::invalid_argument);
    CHECK_THROWS_AS(IP_Address::from_string("255.255.255..255") ,std::invalid_argument);
    CHECK_THROWS_AS(IP_Address::from_string("255.255..255") ,std::invalid_argument);
    IP_Address ip1 = IP_Address::from_string("255.255.255.255");
    REQUIRE(ip1.to_string() == "255.255.255.255");
    IP_Address ip2 = IP_Address::from_string("0.1.2.3");
    REQUIRE(ip2.to_string() == "0.1.2.3");
}

#endif

