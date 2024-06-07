#include "body_size.hpp"
#include "utility.hpp"
#include "global.hpp"
#include <algorithm>
#include <iostream>

BodySize::BodySize() : size(0)
{
    ;
}

BodySize::BodySize(size_t size) : size(size)
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
    //BodySize object;
    if(file_size <= 0 || (size_t)file_size >= MAX_BODY_SIZE+1u)
    {
        ERROR("BodySize error: valid file size is 1 to " + Utility::to_string(MAX_BODY_SIZE) + ". but input file_size:" + Utility::to_string(file_size));
        throw std::invalid_argument("BodySize invalid argument error");
    }
    return (BodySize(file_size));
}


static bool check_over(ssize_t value, ssize_t unit)
{
    if((size_t)value <= (MAX_BODY_SIZE)/unit){
        return true;
    }else{
        ERROR("BodySize error: Too large, please less than " + Utility::to_string(MAX_BODY_SIZE));
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
    }else if(c == 'g' || c == 'G'){
        return g;
    }else{
        ERROR("BodySize error: There are words other than k,m,g. or K,M,G");
        throw std::invalid_argument("BodySize invalid error: c=" + Utility::to_string(c));
    }
    return 1;
}

size_t string_to_number(std::string &file_size, bool is_unit)
{
    std::string::iterator unit = std::find_if(file_size.begin(), file_size.end(), is_notdigit);
    ssize_t tmp;
    ssize_t unit_size;
    if(unit != file_size.end())
    {
        if(is_unit == false){
            ERROR("BodySize error only number:" + file_size);
            throw std::invalid_argument("BodySize invalid error:");
        }
        if(unit != file_size.end()-1){
            ERROR("BodySize error: There are words other than k,m,g. or K,M,G is not unittfix:" + file_size);
            throw std::invalid_argument("BodySize invalid error:");
        }
        size_t len = (unit-file_size.begin());
        std::string amount = file_size.substr(0, len);
        if(amount.size() <= 20){
            tmp = Utility::to_ssize_t(amount);
        }
        unit_size = get_unit_size(*unit);
    }else{
        if(file_size.size() <= 20){
            tmp = Utility::to_ssize_t(file_size);
        }else{
            ERROR("BodySize error: body size is too large:" + file_size);
            throw std::invalid_argument("BodySize invalid error");
        }
        unit_size = 1;
    }
    if(check_over(tmp, unit_size)){
        tmp *= unit_size;
    }
    return (tmp);
}

BodySize BodySize::from_string(std::string &file_size)
{
    size_t size = string_to_number(file_size, true);
    return from_number(size);
}

BodySize BodySize::from_number_string(std::string &file_size)
{
    size_t size = string_to_number(file_size, false);
    return from_number(size);

}

BodySize BodySize::from_string(std::string const &const_file_size)
{
    size_t size = string_to_number(const_cast<std::string&>(const_file_size), true);
    return from_number(size);
}

size_t BodySize::to_number() const
{
    return (size);
}

#ifdef UNIT_TEST
#include "doctest.h"

TEST_CASE("HTTP_VERSION")
{
    CHECK_THROWS_AS(BodySize::from_string("-1"), std::invalid_argument);
    CHECK_THROWS_AS(BodySize::from_string("0"), std::invalid_argument);
    CHECK(BodySize::from_string("1").to_number() == 1);
    CHECK(BodySize::from_string("34359738368").to_number() == 34359738368ul);
    CHECK(BodySize::from_string("34359738367").to_number() == 34359738367ul);
    CHECK_THROWS_AS(BodySize::from_string("34359738369"), std::invalid_argument);

    CHECK(BodySize::from_string("33554431k").to_number() == 34359737344ul);
    CHECK(BodySize::from_string("33554432k").to_number() == 34359738368ul);
    CHECK_THROWS_AS(BodySize::from_string("-1k").to_number(), std::invalid_argument);
    CHECK_THROWS_AS(BodySize::from_string("0k").to_number(), std::invalid_argument);
    CHECK_THROWS_AS(BodySize::from_string("33554433k").to_number(), std::invalid_argument);
    CHECK(BodySize::from_string("33554431K").to_number() == 34359737344ul);
    CHECK(BodySize::from_string("33554432K").to_number() == 34359738368ul);
    CHECK_THROWS_AS(BodySize::from_string("33554433K").to_number(), std::invalid_argument);

    CHECK(BodySize::from_string("32767m").to_number() == 34358689792ul );
    CHECK(BodySize::from_string("32768m").to_number() == 34359738368ul);
    CHECK_THROWS_AS(BodySize::from_string("-1m").to_number(), std::invalid_argument);
    CHECK_THROWS_AS(BodySize::from_string("0m").to_number(), std::invalid_argument);
    CHECK_THROWS_AS(BodySize::from_string("32769m").to_number(), std::invalid_argument);
    CHECK(BodySize::from_string("32767M").to_number() == 34358689792ul );
    CHECK(BodySize::from_string("32768M").to_number() == 34359738368ul);
    CHECK_THROWS_AS(BodySize::from_string("32769M").to_number(), std::invalid_argument);
    CHECK_THROWS_AS(BodySize::from_string("3333333333333333333M").to_number(), std::invalid_argument);

    CHECK(BodySize::from_string("31g").to_number() == 33285996544ul);
    CHECK(BodySize::from_string("32g").to_number() == 34359738368ul);
    CHECK_THROWS_AS(BodySize::from_string("-1g").to_number(), std::invalid_argument);
    CHECK_THROWS_AS(BodySize::from_string("0g").to_number(), std::invalid_argument);
    CHECK_THROWS_AS(BodySize::from_string("33g").to_number(), std::invalid_argument);
    CHECK(BodySize::from_string("31G").to_number() == 33285996544ul);
    CHECK(BodySize::from_string("32G").to_number() == 34359738368ul);
    CHECK_THROWS_AS(BodySize::from_string("33G").to_number(), std::invalid_argument);
    CHECK_THROWS_AS(BodySize::from_string("3333333333333333333G").to_number(), std::invalid_argument);

    std::string test_str = "32k";
    CHECK_THROWS_AS(BodySize::from_number_string(test_str).to_number(), std::invalid_argument);
    test_str = "32M";
    CHECK_THROWS_AS(BodySize::from_number_string(test_str).to_number(), std::invalid_argument);
    test_str = "32g";
    CHECK_THROWS_AS(BodySize::from_number_string(test_str).to_number(), std::invalid_argument);
    test_str = "1";
    CHECK(BodySize::from_number_string(test_str).to_number() == 1ul);
    test_str = "34359738368";
    CHECK(BodySize::from_number_string(test_str).to_number() == 34359738368ul);
    test_str = "34359738369";
    CHECK_THROWS_AS(BodySize::from_number_string(test_str).to_number(), std::invalid_argument);
    test_str = "0";
    CHECK_THROWS_AS(BodySize::from_number_string(test_str).to_number(), std::invalid_argument);


    CHECK_THROWS_AS(BodySize::from_number(-1), std::invalid_argument);
    CHECK_THROWS_AS(BodySize::from_number(0), std::invalid_argument);
    CHECK(BodySize::from_number(1).to_number() == 1);
    CHECK(BodySize::from_number(34359738367).to_number() == 34359738367ul);
    CHECK(BodySize::from_number(34359738368).to_number() == 34359738368ul);
    CHECK_THROWS_AS(BodySize::from_number(34359738369), std::invalid_argument);

}
#endif
