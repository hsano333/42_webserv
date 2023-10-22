#ifndef GLOBAL_HPP
#define GLOBAL_HPP
#include <string>
#include <ctime>
#include "log.hpp"

#define WEBSERV_VERSION "Webserv Version:0.0.1"

#define MAX_FD (200)
#define MAX_BUF (8192)
#define MAX_READ_SIZE (MAX_BUF-10)
#define MAX_READ (1024)
#define MAX_CONFIG_SIZE (MAX_READ_SIZE)
#define LOG_FILE "./webserv.log"
#define LOG_LEVEL_NOT_LOG 1
#define LOG_LEVEL_ERROR 4
#define LOG_LEVEL_WARNING 3
#define LOG_LEVEL_DEBUG 2
#define LOG_LEVEL_INFO 1

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


#define LOG_LEVEL LOG_LEVEL_INFO
#define MYINFO(STR)  (Log::write(__FILE__, __LINE__, "[INFO]", STR, LOG_LEVEL_INFO))
#define DEBUG(STR)  (Log::write(__FILE__, __LINE__, "[DEBUG]",  STR, LOG_LEVEL_DEBUG))
#define WARNING(STR)  (Log::write(__FILE__, __LINE__, "[WARNING]" , STR, LOG_LEVEL_WARNING))
#define ERROR(STR)  (Log::write(__FILE__, __LINE__,  "[ERROR]", STR, LOG_LEVEL_ERROR))

//unit is second
#define TIMEOUT 3
#define CRLF "\r\n"
#define CRLF2 "\r\n\r\n"


#endif
