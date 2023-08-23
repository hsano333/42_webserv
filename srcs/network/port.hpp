#ifndef PORT_HPP
#define PORT_HPP

#include <string>

class Port
{
    public:
        Port();
        Port(Port const &port);
        ~Port();
        static Port from_string(std::string const &port);
        static Port from_int(int port);
        std::string to_string() const;
        std::string to_string();
        const char* c_str() const;
        const char* c_str();
        bool is_valid();
        Port& operator=(Port const &port);
        bool operator==(Port const &port) const;
        bool operator==(Port const &port);
        bool operator<(Port const &port) const;
        bool operator<(Port const &port);

    private:
        int port;
};

#endif
