#ifndef STATUS_CODE_HPP
#define STATUS_CODE_HPP

#include <map>
#include <string>
class StatusCode
{
  public:
    StatusCode();
    ~StatusCode();
    static std::map<int, std::string> _status_code_strs;
    static void init();
    // static std::string get_string_of(int status_code);
};
#endif
