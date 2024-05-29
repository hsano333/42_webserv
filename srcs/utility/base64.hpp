#ifndef BASE64_HPP
#define BASE64_HPP

#include <string>

class Base64
{
  public:
    static std::string encode(std::string const &str);
    static std::string decode(std::string const &str);
};

#endif
