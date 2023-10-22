#ifndef URI_HPP
#define URI_HPP
#include <string>
#include <vector>
#include <map>
#include "split.hpp"

class Request;

class URI
{
  public:
    URI();
    URI(const URI& uri);
    URI& operator=(URI const &uri);
    ~URI();

    static URI from_string(std::string const &str);
    std::string const &raw() const;
    std::string const &path() const;
    const Split &splited_path() const;
    void set_path_info(std::string &path);
    std::string const &get_path_info() const;
    void print_info() const;

  private:
    std::string raw_;
    std::string path_;
    std::string path_info_;
    Split query;
    Split path_sp;
    Split encoded_path_sp;
    void uri_encode(std::string const &raw_uri);
};

std::ostream& operator<<(std::ostream& os, const  URI&uri);

#endif
