#ifndef STATUS_CODE
#define STATUS_CODE
#include <string>
#include <map>


class StatusCode
{
    public:
        StatusCode();
        StatusCode(StatusCode const &code);
        StatusCode &operator=(StatusCode const &code);
        ~StatusCode();
        static StatusCode from_int(int code);
        static StatusCode from_string(std::string const &code);
        std::string to_string() const;
        const char *message() const;
        int to_int() const;
        bool operator==(StatusCode const &code);
        bool operator<(StatusCode const &code);
        bool operator==(StatusCode const &code) const;
        bool operator<(StatusCode const &code) const;


        bool operator==(int code) const;
        bool operator<(int code) const;
        bool operator<=(int code) const;
        bool operator>(int code) const;
        bool operator>=(int code) const;

        static std::map<int, std::string> code_message_list;
        static void init_http_status_message();


    private:
        int code;

};

#endif
