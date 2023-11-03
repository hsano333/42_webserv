#include "body_size.hpp"
#include "utility.hpp"
#include "global.hpp"
#include <algorithm>
#include <iostream>

BodySize::BodySize() : size(0)
{
    ;
}

BodySize::~BodySize()
{
    ;
}

bool is_notdigit(const char &c)
{
    if('0' <= c && c <= '9'){
        return false;
    }
    return true;
}


BodySize BodySize::from_number(ssize_t file_size)
{
    BodySize object;
    if(file_size <= 0 || file_size > INT_MAX+1u)
    {
        ERROR("BodySize error: valid file size is 1 to 2GB. input file_size:" + Utility::to_string(file_size));
        throw std::invalid_argument("BodySize invalid argument error");
    }
    object.size = file_size;
    return (object);
}


static bool check_over(ssize_t value, ssize_t unit)
{
    if(value <= LONG_MAX/unit){
        return true;
    }else{
        ERROR("BodySize error: valid file size is 1 to 2GB. input");
        throw std::invalid_argument("BodySize invalid argument error");
    }
    return false;
}

static ssize_t get_unit_size(char c)
{
    ssize_t k = 1024;
    ssize_t m = k*k;
    ssize_t g = k*m;
    if(c == 'k' || c == 'K'){
        return k;
    }else if(c == 'm' || c == 'M'){
        return m;
        /*
        if(tmp <= LONG_MAX/m){
            tmp *= m;
        }else{
            ERROR("BodySize error: valid file size is 1 to INT_MAX-1. input file_size:" + Utility::to_string(file_size));
            throw std::invalid_argument("BodySize invalid argument error");
        }
        */
    }else if(c == 'g' || c == 'G'){
        return g;
    }
    return 1;
}

BodySize BodySize::from_string(std::string &file_size)
{
    BodySize object;
    std::string::iterator unit = std::find_if(file_size.begin(), file_size.end(), is_notdigit);
    ssize_t tmp;
    ssize_t unit_size;
    if(unit != file_size.end())
    {
        if(unit != file_size.end()-1){
            ERROR("BodySize error: There are words other than k,m,g. or k,m,g is not unittfix");
            throw std::invalid_argument("BodySize invalid error");
        }
        size_t len = (unit-file_size.begin());
        std::string amount = file_size.substr(0, len);
        std::cout << amount << std::endl;
        if(amount.size() <= 10){
            tmp = Utility::to_ssize_t(amount);
        }
    }else{
        if(file_size.size() <= 10){
            tmp = Utility::to_ssize_t(file_size);
        }
    }
    unit_size = get_unit_size(*unit);
    if(check_over(tmp, unit_size)){
        tmp *= unit_size;
    }
    return from_number(tmp);
}


BodySize BodySize::from_string(std::string const &const_file_size)
{
    BodySize object;
    std::string file_size = const_file_size;
    return (from_string(file_size));
}

size_t BodySize::to_number() const
{
    return (size);

}

