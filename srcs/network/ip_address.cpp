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



IP_Address IP_Address::from_int(int i1, int i2, int i3, int i4)
{
    std::string str1;
    std::string str2;
    std::string str3;
    std::string str4;

    if (0 <= i1 && i1 <= 255){
        str1 = Utility::to_string(i1);
    }else{
        ERROR("IP_Address::from_int: invalid argument:" + i1);
        throw std::invalid_argument("IP_Address class: invalid argument");
    }
    if (0 <= i2 && i2 <= 255){
        str2 = Utility::to_string(i2);
    }else{
        ERROR("IP_Address::from_int: invalid argument:" + i2);
        throw std::invalid_argument("IP_Address class: invalid argument");
    }
    if (0 <= i3 && i3 <= 255){
        str3 = Utility::to_string(i3);
    }else{
        ERROR("IP_Address::from_int: invalid argument:" + i3);
        throw std::invalid_argument("IP_Address class: invalid argument");
    }
    if (0 <= i4 && i4 <= 255){
        str4 = Utility::to_string(i4);
    }else{
        ERROR("IP_Address::from_int: invalid argument:" + i4);
        throw std::invalid_argument("IP_Address class: invalid argument");
    }
    std::string const addr = str1 + "." + str2 + "." + str3 + "." + str4;
    return (IP_Address::from_string(addr));
}


#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
IP_Address IP_Address::from_name(std::string const &hostname)
{
    struct addrinfo *res;
    struct addrinfo hints;

    Utility::memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname.c_str(), NULL, &hints, &res) < 0){
        ERROR("IP_Address::from_name: invalid argument:" + hostname);
        throw std::invalid_argument("IP_Address class: invalid argument");
    }
    IP_Address addr;
    bool error = false;
    for(; res; res = res->ai_next){
        try{
            int i1 = res->ai_addr->sa_data[2];
            int i2 = res->ai_addr->sa_data[3];
            int i3 = res->ai_addr->sa_data[4];
            int i4 = res->ai_addr->sa_data[5];

            addr = IP_Address::from_int(i1, i2, i3, i4);
            error = false;
            break;
        }catch (std::invalid_argument &e){
            error = true;
            continue;
        }
    }
    freeaddrinfo(res);
    if(error){
        ERROR("IP_Address::from_name: invalid argument:" + hostname);
        throw std::invalid_argument("IP_Address class: invalid argument");
    }
    return addr;
}

IP_Address IP_Address::from_string(std::string const &str)
{
    IP_Address object;
    Split number(str, ".");
    if(number.size() != 4){
        WARNING("IP_Address class: invalid argument:" + str);
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
        WARNING("IP_Address class: invalid argument:" + str);
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

IP_Address IP_Address::from_string_or_name(std::string const &str)
{
    try{
        return (IP_Address::from_string(str));
    }catch(std::invalid_argument &e){
        try{
            return (IP_Address::from_name(str));
        }catch(std::invalid_argument &e){
            ERROR("IP_Address::from_string_or_name(): string is not ip address :" + str);
            throw std::invalid_argument("IP_Address::from_string_or_name(): string is not ip address");
        }
    }
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

bool IP_Address::operator==(IP_Address const ip)
{
    return (this->to_uint() == ip.to_uint());
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

