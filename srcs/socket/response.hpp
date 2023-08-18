#ifndef RESPONSE_HPP
#define RESPONSE_HPP


#include "config.hpp"
#include "cgi.hpp"
#include "request.hpp"
#include "socket_data.hpp"
#include "config_location.hpp"
#include "request.hpp"


#include <string>
#include <sstream>
#include <map>
#include <fstream>

enum SEND_STATE{
    ERROR,
    STILL_NOT_SEND,
    SENT_STATUS_LINE,
    SENT_HEADER,
    OPEND_FILE,
    SENT_BODY,
    CLOSE,
};


#define MESSAGE200 ("OK")
#define MESSAGE204 ("No Content")
#define MESSAGE206 ("Partial Content")
#define MESSAGE301 ("Moved Permanently")
#define MESSAGE302 ("Found")
#define MESSAGE303 ("See Other")
#define MESSAGE304 ("Not Modified")
#define MESSAGE307 ("Temporary Redirect")
#define MESSAGE400 ("Bad Request")
#define MESSAGE401 ("Unauthorized")
#define MESSAGE403 ("Forbidden")
#define MESSAGE404 ("Not Found")
#define MESSAGE500 ("Internal Server Error")
#define MESSAGE503 ("Service Unavailable")
#define MESSAGEUnknown ("Unknown Status")



class Response : public SocketData
{
public:
    Response();
    Response(Request* request);
    Response(int status_code);
    virtual ~Response();
    Response(Request* request, CGI* cgi);

    map<string, string> _headers;

    void set_header(const std::string& key, const std::string& value);
    void set_body(const std::string& body);
    std::string get_status_message(int code);
    virtual ssize_t get_data(char** data);
    virtual bool open_responsed_file();
    virtual void close_responsed_file();
    std::string get_error();
  private:
    Request* _request;
	bool	_auto_index;
    std::string _file_path;
    int _code;
  protected:
    SEND_STATE _send_state;
    bool _all_sent;
    int _fd;
    string _status_line;
    std::string &get_filepath();
    std::string &get_status_line();
    void change_status_line();
    void check_error();
    void set_status(int code);
    bool _err;
};


#endif /* RESPONSE_HPP */
