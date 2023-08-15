#ifndef REQUEST_CGI_HPP
#define REQUEST_CGI_HPP
#include "request.hpp"
#include "port.hpp"
#include <string>

class RequestCGI : public Request
{
  public:
    RequestCGI(int fd);
    virtual ~RequestCGI();
    bool exist_ContentTypeHeader();
    bool exist_LocationHeader();
    bool exist_ContentLengthHeader();
    int get_status_code();
    void set_status_code(int status_code);
    std::string& get_status_str();
    Port& get_header_line();
    int get_extra_buf(char **data);
    bool has_extra_buf();
    int read_result(char** buf, int size);
    void get_concat_header(string &header);
    bool exist_StatusHeader();
    virtual bool is_cgi();
    virtual bool is_full_body_loaded() const;

  private:
    //int _fd;
    //int _file_fd;
    int _status_code;
    std::string _status_str;
    virtual void parse();
    virtual void parse_request_line();
    std::string _header;
    bool _has_extra_buf;
    bool _is_closed;
};

#endif
