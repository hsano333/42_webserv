#ifndef HTTP_VERSION_HPP
#define HTTP_VERSION_HPP
#include <string>

class HttpVersion
{
    public:
        HttpVersion();
        ~HttpVersion();
        HttpVersion(HttpVersion const &version);
        HttpVersion& operator=(HttpVersion const &version);
        static HttpVersion from_string(std::string const &str);
        std::string to_string() const;
    private:
        std::string version;

};

#endif
