
#ifndef GLOBAL_HPP
#define GLOBAL_HPP
#include <string>
#include <ctime>
#include "log.hpp"

#define WEBSERV_VERSION "Webserv Version:1.0.0"

#define MAX_FD (200)
#define MAX_BUF (8192)
#define MAX_URI (1500)
#define MAX_REAUEST_EXCEPT_BODY (MAX_BUF-1)
//#define MAX_READ_SIZE (MAX_BUF-10)
//1024+512
#define MAX_READ_SIZE (MAX_URI + 10)
//#define MAX_READ (1024)
#define MAX_CONFIG_SIZE (MAX_READ_SIZE)
#define LOG_FILE "./webserv.log"
#define LOG_LEVEL_NOT_LOG 5
#define LOG_LEVEL_ERROR 4
#define LOG_LEVEL_WARNING 3
#define LOG_LEVEL_DEBUG 2
#define LOG_LEVEL_INFO 1

//#define LOG_LEVEL LOG_LEVEL_NOT_LOG
#define LOG_LEVEL LOG_LEVEL_INFO

#define MESSAGE200 ("OK")
#define MESSAGE204 ("No Content")
#define MESSAGE206 ("Partial Content")
#define MESSAGE207 ("Multi-Status")
#define MESSAGE301 ("Moved Permanently")
#define MESSAGE302 ("Found")
#define MESSAGE303 ("See Other")
#define MESSAGE304 ("Not Modified")
#define MESSAGE307 ("Temporary Redirect")
#define MESSAGE400 ("Bad Request")
#define MESSAGE401 ("Unauthorized")
#define MESSAGE403 ("Forbidden")
#define MESSAGE404 ("Not Found")
#define MESSAGE405 ("Method Not Allowed")
#define MESSAGE408 ("Request Timeout")
#define MESSAGE413 ("Payload Too Large")
#define MESSAGE500 ("Internal Server Error")
#define MESSAGE503 ("Service Unavailable")
#define MESSAGE504 ("Gateway Timeout")
#define MESSAGEUnknown ("Unknown Status")



#define MYINFO(STR)  (Log::write(__FILE__, __LINE__, "[INFO]", STR, LOG_LEVEL_INFO))
#define DEBUG(STR)  (Log::write(__FILE__, __LINE__, "[DEBUG]",  STR, LOG_LEVEL_DEBUG))
#define WARNING(STR)  (Log::write(__FILE__, __LINE__, "[WARNING]" , STR, LOG_LEVEL_WARNING))
#define ERROR(STR)  (Log::write(__FILE__, __LINE__,  "[ERROR]", STR, LOG_LEVEL_ERROR))

//unit is second
#define TIMEOUT 50
#define LF "\n"
#define LF2 "\n\n"
#define CRLF "\r\n"
#define CRLF2 "\r\n\r\n"
//#define NL_CGI "\n"
#define NL_CGI "\r\n"
#define NL2_CGI "\r\n\r\n"
#define NL_OFFSET 4
//#define NL_CGI "\n"
//#define NL2_CGI "\n\n"

//32GB
#define MAX_BODY_SIZE (34359738368ul)

#define HTTP_VERSION "HTTP/1.1"


#endif
