#ifndef BASE64_HPP
#define BASE64_HPP

#include <string>

class Base64
{
  public:
    static std::string encode(std::string);
    static std::string decode(std::string);
};

#endif
