#ifndef STATUS_CODE
#define STATUS_CODE
#include <string>


class StatusCode
{
    public:
        StatusCode();
        ~StatusCode();
        static StatusCode from_int(int code);
        static StatusCode from_string(std::string const &code);
        std::string to_string();
        const char *message();
        int to_int() const;
        bool operator==(StatusCode const &code);
        bool operator<(StatusCode const &code);
        bool operator==(StatusCode const &code) const;
        bool operator<(StatusCode const &code) const;

    private:
        int code;

};

#endif
