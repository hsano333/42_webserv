#ifndef RESPONSE_CGI_HPP
#define RESPONSE_CGI_HPP
#include "response.hpp"
#define UNKNOWN_CODE (0)

class ResponseCGI : public Response
{
  public:
    ResponseCGI(CGI* cgi);
    virtual ~ResponseCGI();
    virtual ssize_t get_data(char** data);
    bool get_header_sent();
    //virtual Request* get_req();
    virtual bool open_responsed_file();
    virtual void close_responsed_file();

  private:
    CGI* _cgi;
    std::string _status_line;
    std::string _header_concat;
    void load_and_set_status_code(std::string& sp);
    int determine_status_code(RequestCGI *result);
    const std::string _header_chunked;
    //void copy_header(string &header);
};
#endif
